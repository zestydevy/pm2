
#ifndef INCLUDE_COLLIDER_HPP
#define INCLUDE_COLLIDER_HPP

#include <nusys.h>

#include "heap.hpp"
#include "linklist.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

#undef a0
#undef a1
#undef a2
#undef b0
#undef b1
#undef b2

// -------------------------------------------------------------------------- //

enum class ECollideType {

  BOX,
  SPHERE,
  CYLINDER

};

#define NUM_COLLIDER_TYPES ((u32)ECollideType::CYLINDER + 1)

// -------------------------------------------------------------------------- //

enum : u32 {

  TAG_UNKNOWN,

  TAG_PLAYER = 0x0000'0001,
  TAG_NESTOBJ = 0x0000'0002,
  TAG_NEST = 0x0000'0004,
};

// -------------------------------------------------------------------------- //

class TCollider {

  public:

  TCollider();
  virtual ~TCollider();

  void initCollider(
    u32 tag, u32 send, u32 receive,
    u32 max_hit, THeap * = nullptr
  );

  // TCollider ** getBlkMap() { return mBlkMap; }

  virtual ECollideType getCollideType() const = 0;
  float getCollideCullRadius() const { return mCullRadius; }
  u32 getCollideTag() const { return mTag; }

  bool isCollideActive() const { return mActive; }
  void setCollideActive(bool active) { mActive = active; }

  TVec3F const & getCollideCenter() const { return mCenter; }
  void setCollideCenter(TVec3F const & pt) { mCenter = pt; }

  virtual float getCollideMinX() const = 0;
  virtual float getCollideMinY() const = 0;
  virtual float getCollideMinZ() const = 0;
  virtual float getCollideMaxX() const = 0;
  virtual float getCollideMaxY() const = 0;
  virtual float getCollideMaxZ() const = 0;
  void calcCollideMinMax(TVec3F *, TVec3F *) const;

  static bool startup(THeap *, u16 num_blk, float blk_sz);
  static void shutdown();

  static void frameBegin(); // resets collider state
  static void frameEnd(); // tests colliders, calls onCollide

  protected:

  bool mActive { true };
  TVec3F mCenter { 0.0F, 0.0F, 0.0F };
  float mCullRadius { 0.0F };
  u32 mSendMask { 0 };
  u32 mReceiveMask { 0 };
  u32 mTag { 0 };

  virtual void onCollide(TCollider *) = 0;

  void updateBlkMap();

  private:

  static TCollider ** sBlkMap;
  static float sBlkMapSz;
  static u32 sNumBlkMap;

  static TDoubleLinkList<TCollider> sColliderList;
  TDoubleLinkListNode<TCollider> mListNode { this };

  TCollider ** mBlkMap { nullptr };
  TCollider * mNext { nullptr };

  TCollider ** mHitArray { nullptr };
  u32 mMaxHitNum { 0 };
  u32 mHitNum { 0 };

  bool pushCollision(TCollider *);
  void notifyCollisions();

  static u32 blkMapIdx(float, float, float);
  static TCollider ** blkMapCell(TVec3F const &);
  static void blkMapSpan(TVec3F const &, float, u32 *, u32 *, u32 *, u32 *);
  static void blkMapLink(TCollider **, TCollider *);
  static void blkMapUnlink(TCollider **, TCollider *);
  static void blkMapTest(TCollider **, TCollider *);

};

// -------------------------------------------------------------------------- //

class TBoxCollider :
  public TCollider
{

  public:

  TBoxCollider() = default;
  virtual ~TBoxCollider() = default;

  virtual ECollideType getCollideType() const override;

  TVec3F const & getCollideSize() const { return mSize; }
  void setCollideSize(TVec3F const & sz);

  virtual float getCollideMinX() const override;
  virtual float getCollideMinY() const override;
  virtual float getCollideMinZ() const override;
  virtual float getCollideMaxX() const override;
  virtual float getCollideMaxY() const override;
  virtual float getCollideMaxZ() const override;

  private:

  TVec3F mSize { 0.0F, 0.0F, 0.0F };

};

// -------------------------------------------------------------------------- //

class TSphereCollider :
  public TCollider
{

  public:

  TSphereCollider() = default;
  virtual ~TSphereCollider() = default;

  virtual ECollideType getCollideType() const override;

  float getCollideRadius() const { return mRadius; }
  void setCollideRadius(float r);

  virtual float getCollideMinX() const override;
  virtual float getCollideMinY() const override;
  virtual float getCollideMinZ() const override;
  virtual float getCollideMaxX() const override;
  virtual float getCollideMaxY() const override;
  virtual float getCollideMaxZ() const override;

  private:

  float mRadius { 0.0F };

};

// -------------------------------------------------------------------------- //

class TCylinderCollider :
  public TCollider
{

  public:

  TCylinderCollider() = default;
  virtual ~TCylinderCollider() = default;

  virtual ECollideType getCollideType() const override;

  float getCollideHeight() const { return mHeight; }
  void setCollideHeight(float ht);

  float getCollideRadius() const { return mRadius; }
  void setCollideRadius(float r);

  virtual float getCollideMinX() const override;
  virtual float getCollideMinY() const override;
  virtual float getCollideMinZ() const override;
  virtual float getCollideMaxX() const override;
  virtual float getCollideMaxY() const override;
  virtual float getCollideMaxZ() const override;

  private:

  float mHeight { 0.0F };
  float mRadius { 0.0F };

  float calcCullRadius() const;

};

// -------------------------------------------------------------------------- //

struct TCollideUtil {

  template<typename T>
  static inline T squared(T x) { return (x * x); }

  template<typename T>
  static inline T clamp(T x, T min, T max) {
    if (x < min) {
      return min;
    }

    if (x > max) {
      return max;
    }

    return x;
  }

  static float calcSqrDist(TVec2F const & from, TVec2F const & to);
  static float calcSqrDist(TVec3F const & from, TVec3F const & to);
  static bool doRangesOverlap(float a0, float b0, float a1, float b1);

  static bool testTwoBoxes(
    TVec3F const & min_a, TVec3F const & max_a,
    TVec3F const & min_b, TVec3F const & max_b
  );

  static bool testBoxSphere2D(
    TVec2F const & min, TVec2F const & max,
    TVec2F const & center, float radius
  );

  static bool testBoxSphere3D(
    TVec3F const & min, TVec3F const & max,
    TVec3F const & center, float radius
  );

  static bool testLineLine2D(
    TVec2F const & a0, TVec2F const & b0,
    TVec2F const & b1, TVec2F const & b2,
    TVec2F * pt = nullptr
  );

  static bool isPtInBox2D(
    TVec2F const & min,
    TVec2F const & max,
    TVec2F const & pt
  );

  static float distPtLine(
    TVec3F const & a, TVec3F const & b,
    TVec3F const & src, TVec3F * dst = nullptr
  );

  static float calcLineSign2D(
    TVec2F const & pt,
    TVec2F const & a,
    TVec2F const & b
  );

  static float distPtPoly(
    TVec3F const & pt,
    TVec3F const & v0,
    TVec3F const & v1,
    TVec3F const & v2
  );

  static bool testBoxBox(TCollider const *, TCollider const *);
  static bool testSphereSphere(TCollider const *, TCollider const *);
  static bool testCylinderCylinder(TCollider const *, TCollider const *);

  static bool testBoxSphere(TCollider const *, TCollider const *);
  static bool testBoxCylinder(TCollider const *, TCollider const *);
  static bool testSphereCylinder(TCollider const *, TCollider const *);

};

// -------------------------------------------------------------------------- //

#endif
