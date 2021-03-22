
#include <math.h>
#include <nusys.h>

#include "heap.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

float * TSine::sSinTable;
u16 TSine::sSinShift;

// -------------------------------------------------------------------------- //

void TMtx44::set(
  float m00, float m01, float m02, float m03,
  float m10, float m11, float m12, float m13,
  float m20, float m21, float m22, float m23,
  float m30, float m31, float m32, float m33
) {
  mData[0][0] = m00;
  mData[0][1] = m01;
  mData[0][2] = m02;
  mData[0][3] = m03;
  mData[1][0] = m10;
  mData[1][1] = m11;
  mData[1][2] = m12;
  mData[1][3] = m13;
  mData[2][0] = m20;
  mData[2][1] = m21;
  mData[2][2] = m22;
  mData[2][3] = m23;
  mData[3][0] = m30;
  mData[3][1] = m31;
  mData[3][2] = m32;
  mData[3][3] = m33;
}

// -------------------------------------------------------------------------- //

void TMtx44::identity() {
  set(
    1.0F, 0.0F, 0.0F, 0.0F,
    0.0F, 1.0F, 0.0F, 0.0F,
    0.0F, 0.0F, 1.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::scale(TVec3F const & v) {
  set(
    v.x(),  0.0F,  0.0F, 0.0F,
     0.0F, v.y(),  0.0F, 0.0F,
     0.0F,  0.0F, v.z(), 0.0F,
     0.0F,  0.0F,  0.0F, 1.0f
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::translate(TVec3F const & v) {
  set(
    1.0F, 0.0F, 0.0F, v.x(),
    0.0F, 1.0F, 0.0F, v.y(),
    0.0F, 0.0F, 1.0F, v.z(),
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::rotateEuler(TVec3S const & r) {
  float const sx = TSine::ssin(r.x());
  float const sy = TSine::ssin(r.y());
  float const sz = TSine::ssin(r.z());
  float const cx = TSine::scos(r.x());
  float const cy = TSine::scos(r.y());
  float const cz = TSine::scos(r.z());

  float const m00 = (cz * cy);
  float const m01 = (sx * cz * sy - cx * sz);
  float const m02 = (cx * cz * sy + sx * sz);
  float const m10 = (sz * cy);
  float const m11 = (sx * sz * sy + cx * cz);
  float const m12 = (cx * sz * sy - sx * cz);
  float const m20 = (-sy);
  float const m21 = (cy * sx);
  float const m22 = (cy * cx);

  set(
    m00,  m01,  m02, 0.0F,
    m10,  m11,  m12, 0.0F,
    m20,  m21,  m22, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::rotateAxis(TVec3F const & axis, s16 angle) {
  float const sin = TSine::ssin(angle);
  float const cos = TSine::scos(angle);
  float const theta = (1.0F - cos);

  float const m00 = (theta * axis.x() * axis.x() + cos);
  float const m01 = (theta * axis.x() * axis.y() - sin * axis.z());
  float const m02 = (theta * axis.x() * axis.z() + sin * axis.y());
  float const m10 = (theta * axis.x() * axis.y() + sin * axis.z());
  float const m11 = (theta * axis.y() * axis.y() + cos);
  float const m12 = (theta * axis.y() * axis.z() - sin * axis.x());
  float const m20 = (theta * axis.x() * axis.z() - sin * axis.y());
  float const m21 = (theta * axis.y() * axis.z() + sin * axis.x());
  float const m22 = (theta * axis.z() * axis.z() + cos);

  set(
     m00,  m01,  m02, 0.0F,
     m10,  m11,  m12, 0.0F,
     m20,  m21,  m22, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::rotateAxisX(s16 angle) {
  float const sin = TSine::ssin(angle);
  float const cos = TSine::scos(angle);

  set(
    1.0F, 0.0F, 0.0F, 0.0F,
    0.0F,  cos, -sin, 0.0F,
    0.0F,  sin,  cos, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::rotateAxisY(s16 angle) {
  float const sin = TSine::ssin(angle);
  float const cos = TSine::scos(angle);

  set(
    cos,  0.0F,  sin, 0.0F,
    0.0F, 1.0F, 0.0F, 0.0F,
    -sin, 0.0F,  cos, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::rotateAxisZ(s16 angle) {
  float const sin = TSine::ssin(angle);
  float const cos = TSine::scos(angle);

  set(
     cos, -sin, 0.0F, 0.0F,
     sin,  cos, 0.0F, 0.0F,
    0.0F, 0.0F, 1.0F, 0.0F,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::transform(
  TVec3F const & t,
  TVec3S const & r,
  TVec3F const & s
) {
  float const sx = TSine::ssin(r.x());
  float const sy = TSine::ssin(r.y());
  float const sz = TSine::ssin(r.z());
  float const cx = TSine::scos(r.x());
  float const cy = TSine::scos(r.y());
  float const cz = TSine::scos(r.z());

  float const m00 = (cz * cy * s.x());
  float const m01 = ((sx * cz * sy - cx * sz) * s.y());
  float const m02 = ((cx * cz * sy + sx * sz) * s.z());
  float const m10 = (sz * cy * s.x());
  float const m11 = ((sx * sz * sy + cx * cz) * s.y());
  float const m12 = ((cx * sz * sy - sx * cz) * s.z());
  float const m20 = (-sy * s.x());
  float const m21 = (cy * sx * s.y());
  float const m22 = (cy * cx * s.z());

  set(
    m00,  m01,  m02, t.x(),
    m10,  m11,  m12, t.y(),
    m20,  m21,  m22, t.z(),
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::lookDir(
  TVec3F const & pos,
  TVec3F const & fw,
  TVec3F const & up
) {
  TVec3F rw, uw;

  rw.cross(up, fw);
  rw.normalize();

  uw.cross(fw, rw);
  uw.normalize();

  set(
    rw.x(), rw.y(), rw.z(), -pos.dot(rw),
    uw.x(), uw.y(), uw.z(), -pos.dot(uw),
    fw.x(), fw.y(), fw.z(), -pos.dot(fw),
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::lookAt(
  TVec3F const & pos,
  TVec3F const & at,
  TVec3F const & up
) {
  TVec3F dir;

  if (pos != at) {
    dir.sub(pos, at);
    dir.normalize();
  } else {
    // default to +Z axis (forward)
    dir.set(0.0F, 0.0F, 1.0F);
  }

  lookDir(pos, dir, up);
}

// -------------------------------------------------------------------------- //

void TMtx44::ortho(
  float l, float r,
  float t, float b,
  float n, float f
) {
  float const wd = (1.0F / (r - l));
  float const ht = (1.0F / (t - b));
  float const z = (1.0F / (f - n));

  float const m00 = (2.0F * wd);
  float const m03 = (-(l + r) * wd);
  float const m11 = (2.0F * ht);
  float const m13 = (-(t + b) * ht);
  float const m22 = (2.0F * z);
  float const m23 = (-(f + n) * z);

  set(
     m00, 0.0F, 0.0F, m03,
    0.0F,  m11, 0.0F, m13,
    0.0F, 0.0F,  m22, m23,
    0.0F, 0.0F, 0.0F, 1.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::frustrum(
  float l, float r,
  float b, float t,
  float n, float f
) {
  float const wd = (1.0F / (r - l));
  float const ht = (1.0F / (t - b));
  float const z = (1.0F / (f - n));

  float const m00 = (2.0F * n * wd);
  float const m02 = ((r + l) * wd);
  float const m11 = (2.0F * n * ht);
  float const m12 = ((t + b) * ht);
  float const m22 = ((f + n) * z);
  float const m23 = (-(2.0F * f * n) * z);

  set(
     m00, 0.0F,  m02, 0.0F,
    0.0F,  m11,  m12, 0.0F,
    0.0F, 0.0F,  m22,  m23,
    0.0F, 0.0F, 1.0F, 0.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::perspective(
  s16 fov_y, float aspect,
  float n, float f
) {
  float const a = TSine::scot(fov_y / 2);
  float const z = (1.0F / (f - z));

  float const m00 = (a / aspect);
  float const m11 = a;
  float const m22 = ((f + z) * z);
  float const m23 = (-(2.0F * f * z) * z);

  set(
     m00, 0.0F, 0.0F, 0.0F,
    0.0F,  m11, 0.0F, 0.0F,
    0.0F, 0.0F,  m22,  m23,
    0.0F, 0.0F, 1.0F, 0.0F
  );
}

// -------------------------------------------------------------------------- //

void TMtx44::add(
  TMtx44 const & a,
  TMtx44 const & b
) {
  for (u32 r = 0; r < 4; ++r) {
    for (u32 c = 0; c < 4; ++c) {
      mData[r][c] = (a(r, c) + b(r, c));
    }
  }
}

// -------------------------------------------------------------------------- //

void TMtx44::sub(
  TMtx44 const & a,
  TMtx44 const & b
) {
  for (u32 r = 0; r < 4; ++r) {
    for (u32 c = 0; c < 4; ++c) {
      mData[r][c] = (a(r, c) - b(r, c));
    }
  }
}

// -------------------------------------------------------------------------- //

void TMtx44::transpose(
  TMtx44 const & src,
  TMtx44 & dst
) {
  for (u32 r = 0; r < 4; ++r) {
    for (u32 c = 0; c < 4; ++c) {
      dst(r, c) = src(c, r);
    }
  }
}

// -------------------------------------------------------------------------- //

void TMtx44::concat(
  TMtx44 const & a,
  TMtx44 const & b,
  TMtx44 & dst
) {
  TMtx44 c;
  TMtx44 * p;

  if (&dst == &a || &dst == &b) {
    p = &c;
  } else {
    p = &dst;
  }

  for (u32 r = 0; r < 4; ++r) {
    for (u32 c = 0; c < 4; ++c) {
      (*p)(r, c) = (
        a(r, 0) * b(0, c) + a(r, 1) * b(1, c) +
        a(r, 2) * b(2, c) + a(r, 3) * b(3, c)
      );
    }
  }

  if (p == &c) {
    dst = c;
  }
}

// -------------------------------------------------------------------------- //

TVec3F TMtx44::mul(TVec3F const & v) const {
  float product[4];
  TVec3F vec;

  for (u32 i = 0; i < 4; ++i) {
    product[i] = (
      mData[i][0] * v.x() +
      mData[i][1] * v.y() +
      mData[i][2] * v.z() +
      mData[i][3] // assume w() = 1
    );
  }

  vec.x() = product[0];
  vec.y() = product[1];
  vec.z() = product[2];

  if (product[3] != 1.0F) {
    vec /= product[3];
  }

  return vec;
}

// -------------------------------------------------------------------------- //

void TMtx44::floatToFixed(
  TMtx44 const & src, Mtx & dst
) {
  TMtx44 xpose;
  transpose(src, xpose);
  guMtxF2L(const_cast<TMtx44 &>(xpose).mData, &dst);
}

// -------------------------------------------------------------------------- //

void TMtx44::fixedToFloat(
  Mtx const & src, TMtx44 & dst
) {
  TMtx44 xpose;
  guMtxL2F(xpose.mData, const_cast<Mtx *>(&src));
  transpose(xpose, dst);
}

// -------------------------------------------------------------------------- //

bool TSine::startup(
  u32 bits, THeap * heap
) {
  if (bits == 0 || bits > 16) {
    return false;
  }

  if (sSinTable != nullptr) {
    return false;
  }

  u32 count = (1 << bits);
  sSinTable = new(heap) float[count];
  sSinShift = (16 - bits);

  constexpr auto kTwoPi = static_cast<float>(M_PI * 2.0);
  auto const factor = (kTwoPi / static_cast<float>(count));

  for (u32 i = 0; i < count; ++i) {
    sSinTable[i] = sinf(static_cast<float>(i) * factor);
  }

  return true;
}

// -------------------------------------------------------------------------- //

void TSine::shutdown() {
  if (sSinTable == nullptr) {
    return;
  }

  delete[] sSinTable;
  sSinTable = nullptr;
  sSinShift = 0;
}

// -------------------------------------------------------------------------- //

float TSine::ssin(s16 x) {
  return sSinTable[static_cast<u16>(x) >> sSinShift];
}

// -------------------------------------------------------------------------- //

float TSine::scos(s16 x) {
  return sSinTable[static_cast<u16>(16384 - x) >> sSinShift];
}

// -------------------------------------------------------------------------- //

float TSine::stan(s16 x) {
  return (ssin(x) / scos(x));
}

// -------------------------------------------------------------------------- //

float TSine::scot(s16 x) {
  return (scos(x) / ssin(x));
}

// -------------------------------------------------------------------------- //

s16 TSine::atan2(float y, float x) {
  return fromRad(atan2f(y, x));
}

// -------------------------------------------------------------------------- //

s16 TSine::asin(float y) {
  return fromRad(asinf(y));
}

// -------------------------------------------------------------------------- //

s16 TSine::acos(float x) {
  return fromRad(acosf(x));
}

// -------------------------------------------------------------------------- //
