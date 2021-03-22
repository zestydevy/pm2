
#ifndef INCLUDE_MATH_HPP
#define INCLUDE_MATH_HPP

#include <ultra64.h>

#include "heap.hpp"
#include "util.hpp"

// -------------------------------------------------------------------------- //

template<typename T>
class TMath {

  public:

  static constexpr T epsilon() { return (T)1; }
  static constexpr T zero() { return (T)0; }
  static constexpr T one() { return (T)1; }

  static T abs(T x) { return (x >= zero() ? x : -x); }

  static inline T sqrt(T x) { return x; } // TODO integer sqrt

  static inline T mod(T x, T y) { return (x % y); }

  static inline T min(T x, T y) { return (x < y ? x : y); }
  static inline T max(T x, T y) { return (x > y ? x : y); }

  static inline T clamp(T x, T a, T b) {
    return min(max(x, a), b);
  }

};

// -------------------------------------------------------------------------- //

template<>
class TMath<float> {

  public:

  static constexpr float epsilon() { return 0.0001F; }
  static constexpr float zero() { return 0.0F; }
  static constexpr float one() { return 1.0F; }

  static float abs(float x) { return (x >= 0.0F ? x : -x); }

  static inline float sqrt(float x) { return sqrtf(x); }

  static inline float mod(float x, float y) {
    return (x - static_cast<u64>(x / y) * y);
  }

  static inline float min(float x, float y) { return (x < y ? x : y); }
  static inline float max(float x, float y) { return (x > y ? x : y); }

  static inline float clamp(float x, float a, float b) {
    return min(max(x, a), b);
  }

};

// -------------------------------------------------------------------------- //

template<typename T> class TVec2;
template<typename T> class TVec3;

// -------------------------------------------------------------------------- //

template<typename T>
class TVec2 {

  public:

  TVec2() = default;

  template<typename T2>
  TVec2(T2 x, T2 y) { set(x, y); }

  template<typename T2>
  TVec2(TVec2<T2> const & v) { set<T2>(v.x(), v.y()); }

  T & x() { return mData[0]; }
  T x() const { return mData[0]; }

  T & y() { return mData[1]; }
  T y() const { return mData[1]; }

  T * data() { return mData; }
  T const * data() const { return mData; }

  T & operator[](u32 n) { return mData[n]; }
  T operator[](u32 n) const { return mData[n]; }

  template<typename T2>
  void set(T2 x, T2 y) {
    mData[0] = static_cast<T>(x);
    mData[1] = static_cast<T>(y);
  }

  // addition

  void add(TVec2<T> const & v) { add(*this, v); }

  void add(TVec2<T> const & a, TVec2<T> const & b) {
    set((a.x() + b.x()), (a.y() + b.y()));
  }

  TVec2<T> & operator+=(TVec2<T> const & v) {
    add(*this, v);
    return *this;
  }

  // subtraction

  void sub(TVec2<T> const & v) { sub(*this, v); }

  void sub(TVec2<T> const & a, TVec2<T> const & b) {
    set((a.x() - b.x()), (a.y() - b.y()));
  }

  TVec2<T> & operator-=(TVec2<T> const & v) {
    sub(*this, v);
    return *this;
  }

  // multiplication

  void mul(T x) { mul(*this, x); }
  void mul(TVec2<T> const & v) { mul(*this, v); }

  void mul(TVec2<T> const & v, T x) {
    set((v.x() * x), (v.y() * x));
  }

  void mul(TVec2<T> const & a, TVec2<T> const & b) {
    set((a.x() * b.x()), (a.y() * b.y()));
  }

  TVec2<T> & operator*=(T x) {
    mul(*this, x);
    return *this;
  }

  TVec2<T> & operator*=(TVec2<T> const & v) {
    mul(*this, v);
    return *this;
  }

  // division

  void div(T x) { div(*this, x); }
  void div(TVec2<T> const & v) { div(*this, v); }

  void div(TVec2<T> const & v, T x) {
    set((v.x() / x), (v.y() / x));
  }

  void div(TVec2<T> const & a, TVec2<T> const & b) {
    set((a.x() / b.x()), (a.y() / b.y()));
  }

  TVec2<T> & operator/=(T x) {
    div(*this, x);
    return *this;
  }

  TVec2<T> & operator/=(TVec2<T> const & v) {
    div(*this, v);
    return *this;
  }

  // operators

  T dot(TVec2<T> const & v) const {
    return (x() * v.x() + y() * v.y());
  }

  T getSqrLength() const { return dot(*this); }
  T getLength() const { return TMath<T>::sqrt(getSqrLength()); }

  void setLength(T x) { setLength(*this, x); }

  void setLength(TVec2<float> const & v, T x) {
    T const len = v.getLength();

    if (len != TMath<T>::zero()) {
      mul(v, (x / len));
    } else {
      set(0, 0);
    }
  }

  void normalize() { setLength(*this, TMath<T>::one()); }

  static T distSqr(TVec2<T> const & a, TVec2<T> const & b) {
    TVec2<T> c;
    c.sub(a, b);
    return c.getSqrLength();
  }

  static T dist(TVec2<T> const & a, TVec2<T> const & b) {
    return TMath<T>::sqrt(distSqr(a, b));
  }

  // swizzle

  TVec2<T> xx() const { return TVec2<T> { x(), x() }; }
  TVec2<T> xy() const { return TVec2<T> { x(), y() }; }
  TVec2<T> yx() const { return TVec2<T> { y(), x() }; }
  TVec2<T> yy() const { return TVec2<T> { y(), y() }; }
  TVec3<T> xxx() const { return TVec3<T> { x(), x(), x() }; }
  TVec3<T> xxy() const { return TVec3<T> { x(), x(), y() }; }
  TVec3<T> xyx() const { return TVec3<T> { x(), y(), x() }; }
  TVec3<T> xyy() const { return TVec3<T> { x(), y(), y() }; }
  TVec3<T> yxx() const { return TVec3<T> { y(), x(), x() }; }
  TVec3<T> yxy() const { return TVec3<T> { y(), x(), y() }; }
  TVec3<T> yyx() const { return TVec3<T> { y(), y(), x() }; }
  TVec3<T> yyy() const { return TVec3<T> { y(), y(), y() }; }

  private:

  T mData[2];

};

using TVec2S = TVec2<s16>;
using TVec2F = TVec2<float>;

// -------------------------------------------------------------------------- //

template<typename T1, typename T2>
inline bool operator==(TVec2<T1> const & lhs, TVec2<T2> const & rhs) {
  return (lhs.x() == rhs.x() && lhs.y() == rhs.y());
}

template<typename T1, typename T2>
inline bool operator!=(TVec2<T1> const & lhs, TVec2<T2> const & rhs) {
  return (lhs.x() != rhs.x() || lhs.y() != rhs.y());
}

template<typename T>
inline TVec2<T> operator+(TVec2<T> const & lhs, TVec2<T> const & rhs) {
  TVec2<T> v = lhs;
  v += rhs;
  return v;
}

template<typename T>
inline TVec2<T> operator-(TVec2<T> const & lhs, TVec2<T> const & rhs) {
  TVec2<T> v = lhs;
  v -= rhs;
  return v;
}

template<typename T>
inline TVec2<T> operator*(TVec2<T> const & lhs, T const & rhs) {
  TVec2<T> v = lhs;
  v *= rhs;
  return v;
}

template<typename T>
inline TVec2<T> operator*(T const & lhs, TVec2<T> const & rhs) {
  TVec2<T> v = rhs;
  v *= lhs;
  return v;
}

template<typename T>
inline TVec2<T> operator*(TVec2<T> const & lhs, TVec2<T> const & rhs) {
  TVec2<T> v = lhs;
  v *= rhs;
  return v;
}

template<typename T>
inline TVec2<T> operator/(TVec2<T> const & lhs, T const & rhs) {
  TVec2<T> v = lhs;
  v /= rhs;
  return v;
}

template<typename T>
inline TVec2<T> operator/(TVec2<T> const & lhs, TVec2<T> const & rhs) {
  TVec2<T> v = lhs;
  v /= rhs;
  return v;
}

// -------------------------------------------------------------------------- //

template<typename T>
class TVec3 {

  public:

  TVec3() = default;

  template<typename T2>
  TVec3(T2 x, T2 y, T2 z) { set<T2>(x, y, z); }

  template<typename T2>
  TVec3(TVec3<T2> const & v) { set<T2>(v.x(), v.y() , v.z()); }

  T & x() { return mData[0]; }
  T x() const { return mData[0]; }

  T & y() { return mData[1]; }
  T y() const { return mData[1]; }

  T & z() { return mData[2]; }
  T z() const { return mData[2]; }

  T * data() { return mData; }
  T const * data() const { return mData; }

  T & operator[](u32 n) { return mData[n]; }
  T operator[](u32 n) const { return mData[n]; }

  template<typename T2>
  void set(T2 x, T2 y, T2 z) {
    mData[0] = x;
    mData[1] = y;
    mData[2] = z;
  }

  // addition

  void add(TVec3<T> const & v) { add(*this, v); }

  void add(TVec3<T> const & a, TVec3<T> const & b) {
    set((a.x() + b.x()), (a.y() + b.y()), (a.z() + b.z()));
  }

  TVec3<T> & operator+=(TVec3<T> const & v) {
    add(*this, v);
    return *this;
  }

  // subtraction

  void sub(TVec3<T> const & v) { sub(*this, v); }

  void sub(TVec3<T> const & a, TVec3<T> const & b) {
    set((a.x() - b.x()), (a.y() - b.y()), (a.z() - b.z()));
  }

  TVec3<T> & operator-=(TVec3<T> const & v) {
    sub(*this, v);
    return *this;
  }

  // multiplication

  void mul(T x) { mul(*this, x); }
  void mul(TVec3<T> const & v) { mul(*this, v); }

  void mul(TVec3<T> const & v, T x) {
    set((v.x() * x), (v.y() * x), (v.z() * x));
  }

  void mul(TVec3<T> const & a, TVec3<T> const & b) {
    set((a.x() * b.x()), (a.y() * b.y()), (a.z() * b.z()));
  }

  TVec3<T> & operator*=(T x) {
    mul(*this, x);
    return *this;
  }

  TVec3<T> & operator*=(TVec3<T> const & v) {
    mul(*this, v);
    return *this;
  }

  // division

  void div(T x) { div(*this, x); }
  void div(TVec3<T> const & v) { div(*this, v); }

  void div(TVec3<T> const & v, T x) {
    set((v.x() / x), (v.y() / x), (v.z() / x));
  }

  void div(TVec3<T> const & a, TVec3<T> const & b) {
    set((a.x() / b.x()), (a.y() / b.y()), (a.z() / b.z()));
  }

  TVec3<T> & operator/=(T x) {
    div(*this, x);
    return *this;
  }

  TVec3<T> & operator/=(TVec3<T> const & v) {
    div(*this, v);
    return *this;
  }

  // operators

  void cross(TVec3<T> const & a, TVec3<T> const & b) {
    set(
      (a.y() * b.z() - a.z() * b.y()),
      (a.z() * b.x() - a.x() * b.z()),
      (a.x() * b.y() - a.y() * b.x())
    );
  }

  TVec3<T> cross(TVec3<T> const & v) const {
    TVec3<T> p;
    p.cross(*this, v);
    return p;
  }

  T dot(TVec3<T> const & v) const {
    return (x() * v.x() + y() * v.y() + z() * v.z());
  }

  void lerp(TVec3<T> const & end, float percent) {
    *this = *this + percent * (end - *this);
  }

  void moveTowards(TVec3<T> const & end, float maxDistance) {
    TVec3<T> a = end - *this;
    float mag = a.getLength();
    if (mag <= maxDistance || mag == 0.0f)
      *this = end;
    else
      *this = *this + a / mag * maxDistance;
  }

  void lerpTime(TVec3<T> const & end, float percent, float interval) {
    float dist = (end - *this).getLength();
    moveTowards(end, dist * percent * interval * 60.0f);
  }

  T getSqrLength() const { return dot(*this); }
  T getLength() const { return TMath<T>::sqrt(getSqrLength()); }

  void setLength(T x) { setLength(*this, x); }

  void setLength(TVec3<float> const & v, T x) {
    T const len = v.getLength();

    if (len != TMath<T>::zero()) {
      mul(v, (x / len));
    } else {
      set(0, 0, 0);
    }
  }

  void normalize() { setLength(*this, TMath<T>::one()); }

  static T distSqr(TVec3<T> const & a, TVec3<T> const & b) {
    TVec3<T> c;
    c.sub(a, b);
    return c.getSqrLength();
  }

  static T dist(TVec3<T> const & a, TVec3<T> const & b) {
    return TMath<T>::sqrt(distSqr(a, b));
  }

  // swizzle

  TVec2<T> xx() const { return TVec2<T> { x(), x() }; }
  TVec2<T> xy() const { return TVec2<T> { x(), y() }; }
  TVec2<T> xz() const { return TVec2<T> { x(), z() }; }
  TVec2<T> yx() const { return TVec2<T> { y(), x() }; }
  TVec2<T> yy() const { return TVec2<T> { y(), y() }; }
  TVec2<T> yz() const { return TVec2<T> { y(), z() }; }
  TVec2<T> zx() const { return TVec2<T> { z(), x() }; }
  TVec2<T> zy() const { return TVec2<T> { z(), y() }; }
  TVec2<T> zz() const { return TVec2<T> { z(), z() }; }
  TVec3<T> xxx() const { return TVec3<T> { x(), x(), x() }; }
  TVec3<T> xxy() const { return TVec3<T> { x(), x(), y() }; }
  TVec3<T> xxz() const { return TVec3<T> { x(), x(), z() }; }
  TVec3<T> xyx() const { return TVec3<T> { x(), y(), x() }; }
  TVec3<T> xyy() const { return TVec3<T> { x(), y(), y() }; }
  TVec3<T> xyz() const { return TVec3<T> { x(), y(), z() }; }
  TVec3<T> xzx() const { return TVec3<T> { x(), z(), x() }; }
  TVec3<T> xzy() const { return TVec3<T> { x(), z(), y() }; }
  TVec3<T> xzz() const { return TVec3<T> { x(), z(), z() }; }
  TVec3<T> yxx() const { return TVec3<T> { y(), x(), x() }; }
  TVec3<T> yxy() const { return TVec3<T> { y(), x(), y() }; }
  TVec3<T> yxz() const { return TVec3<T> { y(), x(), z() }; }
  TVec3<T> yyx() const { return TVec3<T> { y(), y(), x() }; }
  TVec3<T> yyy() const { return TVec3<T> { y(), y(), y() }; }
  TVec3<T> yyz() const { return TVec3<T> { y(), y(), z() }; }
  TVec3<T> yzx() const { return TVec3<T> { y(), z(), x() }; }
  TVec3<T> yzy() const { return TVec3<T> { y(), z(), y() }; }
  TVec3<T> yzz() const { return TVec3<T> { y(), z(), z() }; }
  TVec3<T> zxx() const { return TVec3<T> { z(), x(), x() }; }
  TVec3<T> zxy() const { return TVec3<T> { z(), x(), y() }; }
  TVec3<T> zxz() const { return TVec3<T> { z(), x(), z() }; }
  TVec3<T> zyx() const { return TVec3<T> { z(), y(), x() }; }
  TVec3<T> zyy() const { return TVec3<T> { z(), y(), y() }; }
  TVec3<T> zyz() const { return TVec3<T> { z(), y(), z() }; }
  TVec3<T> zzx() const { return TVec3<T> { z(), z(), x() }; }
  TVec3<T> zzy() const { return TVec3<T> { z(), z(), y() }; }
  TVec3<T> zzz() const { return TVec3<T> { z(), z(), z() }; }

  private:

  T mData[3];

};

using TVec3S = TVec3<s16>;
using TVec3F = TVec3<float>;

// -------------------------------------------------------------------------- //

template<typename T1, typename T2>
inline bool operator==(TVec3<T1> const & lhs, TVec3<T2> const & rhs) {
  return (lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z());
}

template<typename T1, typename T2>
inline bool operator!=(TVec3<T1> const & lhs, TVec3<T2> const & rhs) {
  return (lhs.x() != rhs.x() || lhs.y() != rhs.y() || lhs.z() != rhs.z());
}

template<typename T>
inline TVec3<T> operator+(TVec3<T> const & lhs, TVec3<T> const & rhs) {
  TVec3<T> v = lhs;
  v += rhs;
  return v;
}

template<typename T>
inline TVec3<T> operator-(TVec3<T> const & lhs, TVec3<T> const & rhs) {
  TVec3<T> v = lhs;
  v -= rhs;
  return v;
}

template<typename T>
inline TVec3<T> operator*(TVec3<T> const & lhs, T const & rhs) {
  TVec3<T> v = lhs;
  v *= rhs;
  return v;
}

template<typename T>
inline TVec3<T> operator*(T const & lhs, TVec3<T> const & rhs) {
  TVec3<T> v = rhs;
  v *= lhs;
  return v;
}

template<typename T>
inline TVec3<T> operator*(TVec3<T> const & lhs, TVec3<T> const & rhs) {
  TVec3<T> v = lhs;
  v *= rhs;
  return v;
}

template<typename T>
inline TVec3<T> operator/(TVec3<T> const & lhs, T const & rhs) {
  TVec3<T> v = lhs;
  v /= rhs;
  return v;
}

template<typename T>
inline TVec3<T> operator/(TVec3<T> const & lhs, TVec3<T> const & rhs) {
  TVec3<T> v = lhs;
  v /= rhs;
  return v;
}

// -------------------------------------------------------------------------- //

class TMtx44 {

  public:

  TMtx44() = default;

  float & operator()(u32 r, u32 c) { return mData[r][c]; }
  float operator()(u32 r, u32 c) const { return mData[r][c]; }

  void set(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
  );

  void identity();

  // model matrix

  void scale(TVec3F const &);
  void translate(TVec3F const &);
  void rotateEuler(TVec3S const &);
  void rotateAxis(TVec3F const &, s16);
  void rotateAxisX(s16);
  void rotateAxisY(s16);
  void rotateAxisZ(s16);

  void transform(
    TVec3F const & t,
    TVec3S const & r,
    TVec3F const & s
  );

  // view matrix

  void lookAt(TVec3F const & pos, TVec3F const & at, TVec3F const & up);
  void lookDir(TVec3F const & pos, TVec3F const & fw, TVec3F const & up);

  // projection matrix

  void ortho(float l, float r, float b, float t, float n, float f);
  void frustrum(float l, float r, float b, float t, float n, float f);
  void perspective(s16 fov_y, float aspect, float n, float f);

  // operators

  void add(TMtx44 const & m) { add(*this, m); }
  void add(TMtx44 const & a, TMtx44 const & b);

  void sub(TMtx44 const & m) { sub(*this, m); }
  void sub(TMtx44 const & a, TMtx44 const & b);

  static void transpose(TMtx44 const & src, TMtx44 & dst);
  static void concat(TMtx44 const & a, TMtx44 const & b, TMtx44 & dst);

  TVec3F mul(TVec3F const & v) const;

  static void floatToFixed(TMtx44 const & src, Mtx & dst);
  static void fixedToFloat(Mtx const & src, TMtx44 & dst);

  private:

  float mData[4][4];

};

// -------------------------------------------------------------------------- //

class TSine {

  public:

  static bool startup(u32 bits, THeap * heap = nullptr);
  static void shutdown();

  static float ssin(s16 x);
  static float scos(s16 x);
  static float stan(s16 x);
  static float scot(s16 x);

  static s16 atan2(float y, float x);
  static s16 asin(float y);
  static s16 acos(float x);

  static constexpr float toRad(s16 x) {
    constexpr auto kRatio = (
      static_cast<float>(65536.0 / (M_PI * 2.0))
    );

    return (static_cast<float>(x) * kRatio);
  }

  static constexpr float toDeg(s16 x) {
    constexpr auto kRatio = (
      static_cast<float>(65536.0 / 360.0)
    );

    return (static_cast<float>(x) * kRatio);
  }

  static constexpr float toRad(float x) {
    constexpr auto kRatio = (
      1.0F / static_cast<float>(360.0 / (M_PI * 2.0))
    );

    return (x * kRatio);
  }

  static constexpr float toDeg(float x) {
    constexpr auto kRatio = (
      static_cast<float>(360.0 / (M_PI * 2.0))
    );

    return (x * kRatio);
  }

  static constexpr s16 fromRad(float x) {
    return static_cast<s16>(x / (M_PI * 2.0) * 65536.0);
  }

  static constexpr s16 fromDeg(float x) {
    return static_cast<s16>(x / 360.0F * 65536.0);
  }

  private:

  static float * sSinTable;
  static u16 sSinShift;

};

// -------------------------------------------------------------------------- //

#endif
