
#ifndef INCLUDE_DYNLIST2_HPP
#define INCLUDE_DYNLIST2_HPP

#include <nusys.h>

#include "heap.hpp"

// -------------------------------------------------------------------------- //

class TDynList2 {

  public:

  TDynList2(u32 len = 2048, THeap * heap = nullptr);
  ~TDynList2();

  void reset();
  u32 fetchCmdIndex();

  Gfx * getDL() const;
  Gfx * pushDL();
  Gfx * getHead();

  static Mtx const & getIdentityMtx();

  Mtx const & getProjMtx() const { return mProjMtx; }
  void setProjMtx(Mtx const & mtx) { mProjMtx = mtx; }

  Mtx const & getViewMtx() const { return mViewMtx; }
  void setViewMtx(Mtx const & mtx) { mViewMtx = mtx; }

  inline void flip() { mFrameNum = (mFrameNum + 1) % 4; }

  private:

  Mtx mProjMtx, mViewMtx;
  THeap * mHeap { nullptr };
  Gfx * mGfxAry { nullptr };
  Gfx * mGfxAry2 { nullptr };
  Gfx * mGfxAry3 { nullptr };
  Gfx * mGfxAry4 { nullptr };
  u32 mMaxNumGfx { 0 };
  u32 mNumGfx { 0 };
  u32 mFrameNum{ 0 };

};

// -------------------------------------------------------------------------- //

#endif
