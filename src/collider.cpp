
#include "collider.hpp"
#include "heap.hpp"
#include "linklist.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

using TTestFn = bool (*)(TCollider const *, TCollider const *);

struct TTestInfo {

  TTestFn fn;
  bool swap; // false: fn(a, b) true: fn(b, a)

};

// -------------------------------------------------------------------------- //

TCollider ** TCollider::sBlkMap { nullptr };
float TCollider::sBlkMapSz { 0.0F };
u32 TCollider::sNumBlkMap { 0 };

TDoubleLinkList<TCollider> TCollider::sColliderList;

static TTestInfo sTestTable[NUM_COLLIDER_TYPES * NUM_COLLIDER_TYPES];

// -------------------------------------------------------------------------- //

TCollider::TCollider() {
  sColliderList.link(sColliderList.end(), &mListNode);
}

// -------------------------------------------------------------------------- //

TCollider::~TCollider() {
  sColliderList.unlink(&mListNode);
  delete[] mHitArray;
}

// -------------------------------------------------------------------------- //

void TCollider::initCollider(
  u32 const tag,
  u32 const send,
  u32 const receive,
  u32 const max_hit,
  THeap * const heap
) {
  mTag = tag;
  mSendMask = send;
  mReceiveMask = receive;
  mMaxHitNum = max_hit;
  delete[] mHitArray;
  mHitArray = new(heap) TCollider * [mMaxHitNum];
  mHitNum = 0;
}

// -------------------------------------------------------------------------- //

void TCollider::calcCollideMinMax(
  TVec3F * const min,
  TVec3F * const max
) const {
  if (min != nullptr) {
    min->set(
      getCollideMinX(),
      getCollideMinY(),
      getCollideMinZ()
    );
  }

  if (max != nullptr) {
    max->set(
      getCollideMaxX(),
      getCollideMaxY(),
      getCollideMaxZ()
    );
  }
}

// -------------------------------------------------------------------------- //

bool TCollider::startup(
  THeap * heap, u16 num_blk, float blk_sz
) {
  if (num_blk == 0 || blk_sz <= 0.0F) {
    return false;
  }

  sBlkMap = new(heap) TCollider * [num_blk * num_blk];
  sNumBlkMap = num_blk;
  sBlkMapSz = blk_sz;

  if (sBlkMap == nullptr) {
    shutdown();
    return false;
  }

  for (u32 i = 0; i < (num_blk * num_blk); ++i) {
    sBlkMap[i] = nullptr;
  }

  sColliderList.clear();

  // box -> box
  sTestTable[0] = {           &TCollideUtil::testBoxBox, false };

  // box -> sphere
  sTestTable[1] = {        &TCollideUtil::testBoxSphere, false };

  // box -> cylinder
  sTestTable[2] = {      &TCollideUtil::testBoxCylinder, false };

  // sphere -> box
  sTestTable[3] = {        &TCollideUtil::testBoxSphere,  true };

  // sphere -> sphere
  sTestTable[4] = {     &TCollideUtil::testSphereSphere, false };

  // sphere -> cylinder
  sTestTable[5] = {   &TCollideUtil::testSphereCylinder, false };

  // cylinder -> box
  sTestTable[6] = {      &TCollideUtil::testBoxCylinder,  true };

  // cylinder -> sphere
  sTestTable[7] = {   &TCollideUtil::testSphereCylinder,  true };

  // cylinder -> cylinder
  sTestTable[8] = { &TCollideUtil::testCylinderCylinder, false };

  return true;
}

// -------------------------------------------------------------------------- //

void TCollider::shutdown() {
  delete[] sBlkMap;
  sNumBlkMap = 0;
  sBlkMapSz = 0.0F;
}

// -------------------------------------------------------------------------- //

void TCollider::frameBegin() {
  auto node = sColliderList.begin();

  while (node != sColliderList.end()) {
    node->data->mHitNum = 0;
    node = node->next;
  }
}

// -------------------------------------------------------------------------- //

void TCollider::frameEnd() {
  TDoubleLinkListNode<TCollider> * it;
  u32 l, b, r, t;

  for (
    it = sColliderList.begin();
    (it != sColliderList.end());
    it = it->next
  ) {
    if (!it->data->isCollideActive()) {
      continue;
    }

    blkMapSpan(
      it->data->mCenter,
      it->data->mCullRadius,
      &l, &b, &r, &t
    );

    for (u32 y = b; y <= t; ++y) {
      for (u32 x = l; x <= r; ++x) {
        blkMapTest(&sBlkMap[y * sNumBlkMap + x], it->data);
      }
    }
  }

  for (
    it = sColliderList.begin();
    (it != sColliderList.end());
    it = it->next
  ) {
    if (it->data->mHitNum == 0) {
      continue;
    }

    it->data->notifyCollisions();
  }
}

// -------------------------------------------------------------------------- //

void TCollider::updateBlkMap() {
  if (!mActive) {
    return;
  }

  TCollider ** list = blkMapCell(mCenter);

  if (list == mBlkMap) {
    return;
  }

  if (mBlkMap != nullptr) {
    blkMapUnlink(mBlkMap, this);
  }

  if (list != nullptr) {
    blkMapLink(list, this);
  }
}

// -------------------------------------------------------------------------- //

bool TCollider::pushCollision(
  TCollider * const other
) {
  if (mHitNum >= mMaxHitNum) {
    return false;
  }

  for (u32 i = 0; i < mHitNum; ++i) {
    if (mHitArray[i] == other) {
      return false;
    }
  }

  mHitArray[mHitNum++] = other;
  return true;
}

// -------------------------------------------------------------------------- //

void TCollider::notifyCollisions() {
  for (u32 i = 0; i < mHitNum; ++i) {
    onCollide(mHitArray[i]);
  }
}

// -------------------------------------------------------------------------- //

u32 TCollider::blkMapIdx(
  float x, float half, float i_sz
) {
  auto max = (s32)(sNumBlkMap - 1);
  auto idx = (s32)((half + x) * i_sz);
  return (u32)TMath<s32>::clamp(idx, 0, max);
}

// -------------------------------------------------------------------------- //

TCollider **
TCollider::blkMapCell(
  TVec3F const & pos
) {
  float half = ((float)sNumBlkMap * sBlkMapSz * 0.5F);
  float i_sz = (1.0F / sBlkMapSz);
  u32 x = blkMapIdx(pos.x(), half, i_sz);
  u32 y = blkMapIdx(pos.z(), half, i_sz);
  return &sBlkMap[y * sNumBlkMap + x];
}

// -------------------------------------------------------------------------- //

void TCollider::blkMapSpan(
  TVec3F const & pos, float rd,
  u32 * l, u32 * b, u32 * r, u32 * t
) {
  float half = ((float)sNumBlkMap * sBlkMapSz * 0.5F);
  float i_sz = (1.0F / sBlkMapSz);

  if (l != nullptr) {
    *l = blkMapIdx((pos.x() - rd), half, i_sz);
  }

  if (b != nullptr) {
    *b = blkMapIdx((pos.z() - rd), half, i_sz);
  }

  if (r != nullptr) {
    *r = blkMapIdx((pos.x() + rd), half, i_sz);
  }

  if (t != nullptr) {
    *t = blkMapIdx((pos.z() + rd), half, i_sz);
  }
}

// -------------------------------------------------------------------------- //

void TCollider::blkMapLink(
  TCollider ** list, TCollider * node
) {
  node->mBlkMap = list;
  node->mNext = *list;
  *list = node;
}

// -------------------------------------------------------------------------- //

void TCollider::blkMapUnlink(
  TCollider ** list, TCollider * node
) {
  TCollider * prev = *list;

  if (prev == node) {
    *list = nullptr;
  } else {
    while (prev != nullptr) {
      if (prev->mNext == node) {
        break;
      }

      prev = prev->mNext;
    }

    if (prev != nullptr) {
      prev->mNext = node->mNext;
    }
  }

  node->mBlkMap = nullptr;
  node->mNext = nullptr;
}

// -------------------------------------------------------------------------- //

void TCollider::blkMapTest(
  TCollider ** list, TCollider * a
) {
  TCollider * node = *list;
  TCollider * b;

  while (node != nullptr) {
    b = node;
    node = node->mNext;

    if (!b->isCollideActive()) {
      continue;
    }

    if (b->mHitNum == b->mMaxHitNum) {
      continue;
    }

    if (
      ((a->mReceiveMask & b->mSendMask) == 0) &&
      ((b->mReceiveMask & a->mSendMask) == 0)
    ) {
      continue;
    }

    u32 idx = (
      (u32)a->getCollideType() * NUM_COLLIDER_TYPES +
      (u32)b->getCollideType()
    );

    bool check;

    if (sTestTable[idx].swap) {
      check = sTestTable[idx].fn(b, a);
    } else {
      check = sTestTable[idx].fn(a, b);
    }

    if (!check) {
      continue;
    }

    if ((a->mReceiveMask & b->mSendMask) != 0) {
      a->pushCollision(b);
    }

    if ((b->mReceiveMask & a->mSendMask) != 0) {
      b->pushCollision(a);
    }
  }
}

// -------------------------------------------------------------------------- //

ECollideType
TBoxCollider::getCollideType() const {
  return ECollideType::BOX;
}

// -------------------------------------------------------------------------- //

void TBoxCollider::setCollideSize(
  TVec3F const & sz
) {
  mSize = sz;

  mCullRadius = (0.5F * TMath<float>::max(
    TMath<float>::max(sz.x(), sz.y()), sz.z()
  ));
}

// -------------------------------------------------------------------------- //

float TBoxCollider::getCollideMinX() const {
  return (mCenter.x() - mSize.x() * 0.5F);
}

// -------------------------------------------------------------------------- //

float TBoxCollider::getCollideMinY() const {
  return (mCenter.y() - mSize.y() * 0.5F);
}

// -------------------------------------------------------------------------- //

float TBoxCollider::getCollideMinZ() const {
  return (mCenter.z() - mSize.z() * 0.5F);
}

// -------------------------------------------------------------------------- //

float TBoxCollider::getCollideMaxX() const {
  return (mCenter.x() + mSize.x() * 0.5F);
}

// -------------------------------------------------------------------------- //

float TBoxCollider::getCollideMaxY() const {
  return (mCenter.y() + mSize.y() * 0.5F);
}

// -------------------------------------------------------------------------- //

float TBoxCollider::getCollideMaxZ() const {
  return (mCenter.z() + mSize.z() * 0.5F);
}

// -------------------------------------------------------------------------- //

ECollideType
TSphereCollider::getCollideType() const {
  return ECollideType::SPHERE;
}

// -------------------------------------------------------------------------- //

void TSphereCollider::setCollideRadius(float rd) {
  mCullRadius = mRadius = rd;
}

// -------------------------------------------------------------------------- //

float TSphereCollider::getCollideMinX() const {
  return (mCenter.x() - mRadius);
}

// -------------------------------------------------------------------------- //

float TSphereCollider::getCollideMinY() const {
  return (mCenter.y() - mRadius);
}

// -------------------------------------------------------------------------- //

float TSphereCollider::getCollideMinZ() const {
  return (mCenter.z() - mRadius);
}

// -------------------------------------------------------------------------- //

float TSphereCollider::getCollideMaxX() const {
  return (mCenter.x() + mRadius);
}

// -------------------------------------------------------------------------- //

float TSphereCollider::getCollideMaxY() const {
  return (mCenter.y() + mRadius);
}

// -------------------------------------------------------------------------- //

float TSphereCollider::getCollideMaxZ() const {
  return (mCenter.z() + mRadius);
}

// -------------------------------------------------------------------------- //

ECollideType
TCylinderCollider::getCollideType() const {
  return ECollideType::CYLINDER;
}

// -------------------------------------------------------------------------- //

void TCylinderCollider::setCollideHeight(float ht) {
  mHeight = ht;
  mCullRadius = calcCullRadius();
}

// -------------------------------------------------------------------------- //

void TCylinderCollider::setCollideRadius(float rd) {
  mRadius = rd;
  mCullRadius = calcCullRadius();
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::getCollideMinX() const {
  return (mCenter.x() - mRadius);
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::getCollideMinY() const {
  return (mCenter.y() - mHeight * 0.5F);
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::getCollideMinZ() const {
  return (mCenter.z() - mRadius);
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::getCollideMaxX() const {
  return (mCenter.x() + mRadius);
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::getCollideMaxY() const {
  return (mCenter.y() + mHeight * 0.5F);
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::getCollideMaxZ() const {
  return (mCenter.z() + mRadius);
}

// -------------------------------------------------------------------------- //

float TCylinderCollider::calcCullRadius() const {
  return TMath<float>::max((mHeight * 0.5F), mRadius);
}

// -------------------------------------------------------------------------- //

float TCollideUtil::calcSqrDist(
  TVec2F const & from,
  TVec2F const & to
) {
  TVec2F dist;
  dist.sub(to, from);
  return dist.getSqrLength();
}

// -------------------------------------------------------------------------- //

float TCollideUtil::calcSqrDist(
  TVec3F const & from,
  TVec3F const & to
) {
  TVec3F dist;
  dist.sub(to, from);
  return dist.getSqrLength();
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::doRangesOverlap(
  float const min_a, float const max_a,
  float const min_b, float const max_b
) {
  return (min_b <= max_a && max_b >= min_a);
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testTwoBoxes(
  TVec3F const & min_a, TVec3F const & max_a,
  TVec3F const & min_b, TVec3F const & max_b
) {
  return (
    doRangesOverlap(min_a.x(), max_a.x(), min_b.x(), max_b.x()) &&
    doRangesOverlap(min_a.y(), max_a.y(), min_b.y(), max_b.y()) &&
    doRangesOverlap(min_a.z(), max_a.z(), min_b.z(), max_b.z())
  );
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testBoxSphere2D(
  TVec2F const & min, TVec2F const & max,
  TVec2F const & center, float const radius
) {
  // (Jim Arvo, Graphics Gems 2)
  // find the point in the box closest to center.
  // then check if the point is inside the sphere.
  float d = 0.0F;

  for (u32 i = 0; i < 2; ++i) {
    if (center[i] < min[i]) {
      d += squared(center[i] - min[i]);
    } else if (center[i] > max[i]) {
      d += squared(center[i] - max[i]);
    }
  }

  return (d <= squared(radius));
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testBoxSphere3D(
  TVec3F const & min, TVec3F const & max,
  TVec3F const & center, float const radius
) {
  float d = 0.0F;

  for (u32 i = 0; i < 3; ++i) {
    if (center[i] < min[i]) {
      d += squared(center[i] - min[i]);
    } else if (center[i] > max[i]) {
      d += squared(center[i] - max[i]);
    }
  }

  return (d <= squared(radius));
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testLineLine2D(
  TVec2F const & a0, TVec2F const & b0,
  TVec2F const & a1, TVec2F const & b1,
  TVec2F * pt
) {
  TVec2F ab0, ab1;
  float d, t, u;

  ab0.sub(b0, a0);
  ab1.sub(b1, a1);
  d = (-ab1.x() * ab0.y() + ab0.x() * ab1.y());

  if (TMath<float>::abs(d) < TMath<float>::epsilon()) {
    return false;
  }

  t = ((-ab0.y() * (a0.x() - a1.x()) + ab0.x() * (a0.y() - a1.y())) / d);
  u = (( ab1.x() * (a0.y() - a1.y()) - ab1.y() * (a0.x() - a1.x())) / d);

  if (t < 0.0F || t > 1.0F) {
    return false;
  }

  if (u < 0.0F || u > 1.0F) {
    return false;
  }

  if (pt != nullptr) {
    pt->set(
      (a0.x() + (u * ab0.x())),
      (a0.y() + (u * ab0.y()))
    );
  }

  return true;
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::isPtInBox2D(
  TVec2F const & min,
  TVec2F const & max,
  TVec2F const & pt
) {
  return (
    (min.x() <= pt.x() && pt.x() <= max.x()) &&
    (min.y() <= pt.y() && pt.y() <= max.y())
  );
}

// -------------------------------------------------------------------------- //

float TCollideUtil::distPtLine(
  TVec3F const & a, TVec3F const & b,
  TVec3F const & src, TVec3F * dst
) {
  TVec3F ab, ac;
  float d, l, t = 0.0F;

  ab.sub(b, a);
  ac.sub(src, a);
  l = ab.getSqrLength();

  if (TMath<float>::abs(l) > TMath<float>::epsilon()) {
    t = (ab.dot(ac) / l);
  }

  if (t <= 0.0F) {
    d = TVec3F::dist(src, a);

    if (dst != nullptr) {
      *dst = a;
    }
  } else if (t >= 1.0F) {
    d = TVec3F::dist(src, b);

    if (dst != nullptr) {
      *dst = b;
    }
  } else {
    TVec3F pt;
    pt.mul(ab, t);
    pt.add(a);

    d = TVec3F::dist(src, pt);

    if (dst != nullptr) {
      *dst = pt;
    }
  }

  return d;
}

// -------------------------------------------------------------------------- //

float TCollideUtil::calcLineSign2D(
  TVec2F const & pt,
  TVec2F const & a,
  TVec2F const & b
) {
  return (
    (pt.x() - b.x()) * ( a.y() - b.y()) -
    ( a.x() - b.x()) * (pt.y() - b.y())
  );
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testBoxBox(
  TCollider const * lhs, TCollider const * rhs
) {
  TVec3F min_a, max_a, min_b, max_b;
  lhs->calcCollideMinMax(&min_a, &max_a);
  rhs->calcCollideMinMax(&min_b, &max_b);
  return testTwoBoxes(min_a, max_a, min_b, max_b);
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testSphereSphere(
  TCollider const * lhs, TCollider const * rhs
) {
  auto a = static_cast<TSphereCollider const *>(lhs);
  auto b = static_cast<TSphereCollider const *>(rhs);

  float const max = TCollideUtil::squared(
    a->getCollideRadius() + b->getCollideRadius()
  );

  float const dist = TCollideUtil::calcSqrDist(
    a->getCollideCenter(), b->getCollideCenter()
  );

  return (dist <= max);
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testCylinderCylinder(
  TCollider const * lhs, TCollider const * rhs
) {
  auto a = static_cast<TCylinderCollider const *>(lhs);
  auto b = static_cast<TCylinderCollider const *>(rhs);

  if (!doRangesOverlap(
    a->getCollideMinY(), a->getCollideMaxY(),
    rhs->getCollideMinY(), b->getCollideMaxY()
  )) {
    return false;
  }

  float max, dist;

  max = TCollideUtil::squared(
    a->getCollideRadius() +
    b->getCollideRadius()
  );

  dist = TCollideUtil::calcSqrDist(
    a->getCollideCenter(),
    b->getCollideCenter()
  );

  return (dist <= max);
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testBoxSphere(
  TCollider const * lhs, TCollider const * rhs
) {
  auto a = static_cast<TBoxCollider const *>(lhs);
  auto b = static_cast<TSphereCollider const *>(rhs);

  TVec3F min, max;
  a->calcCollideMinMax(&min, &max);

  return testBoxSphere3D(min, max,
    b->getCollideCenter(),
    b->getCollideRadius()
  );
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testBoxCylinder(
  TCollider const * lhs, TCollider const * rhs
) {
  auto a = static_cast<TBoxCollider const *>(lhs);
  auto b = static_cast<TCylinderCollider const *>(rhs);

  TVec3F min, max;
  a->calcCollideMinMax(&min, &max);

  if (!doRangesOverlap(min.y(), max.y(),
    b->getCollideMinY(),
    b->getCollideMaxY()
  )) {
    return false;
  }

  return testBoxSphere2D(min.xz(), max.xz(),
    b->getCollideCenter().xz(),
    b->getCollideRadius()
  );
}

// -------------------------------------------------------------------------- //

bool TCollideUtil::testSphereCylinder(
  TCollider const * lhs, TCollider const * rhs
) {
  auto sphere = static_cast<TSphereCollider const *>(lhs);
  auto cylinder = static_cast<TCylinderCollider const * >(rhs);

  float d, r, m, min_y, max_y;
  TVec2F a, b;
  TVec3F c;

  c = sphere->getCollideCenter();
  a = cylinder->getCollideCenter().xz();
  b = (c.xz() - a);
  d = b.getSqrLength();
  r = squared(cylinder->getCollideRadius());

  if (d > r) {
    b.normalize();
    c.x() = (a.x() + b.x() * cylinder->getCollideRadius());
    c.z() = (a.y() + b.y() * cylinder->getCollideRadius());
  }

  min_y = cylinder->getCollideMinY();
  max_y = cylinder->getCollideMaxY();
  c.y() = clamp(c.y(), min_y, max_y);
  m = (r + squared(sphere->getCollideRadius()));

  return (calcSqrDist(c, sphere->getCollideCenter()) <= m);
}

// -------------------------------------------------------------------------- //
