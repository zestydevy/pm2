
#include <nusys.h>

#include "dynlist.hpp"
#include "heap.hpp"

// -------------------------------------------------------------------------- //

TDynList2::TDynList2(
  u32 len, THeap * heap
) {
  if (heap == nullptr) {
    heap = THeap::getCurrentHeap();
  }

  mHeap = heap;
  mGfxAry = new(mHeap) Gfx[len];
  mGfxAry2 = new(mHeap) Gfx[len];
  mGfxAry3 = new(mHeap) Gfx[len];
  mGfxAry4 = new(mHeap) Gfx[len];
  mMaxNumGfx = len;

  mProjMtx = getIdentityMtx();
  mViewMtx = getIdentityMtx();
}

// -------------------------------------------------------------------------- //

TDynList2::~TDynList2() {
  delete[] mGfxAry;
  delete[] mGfxAry2;
  delete[] mGfxAry3;
  delete[] mGfxAry4;
}

// -------------------------------------------------------------------------- //

void TDynList2::reset() {
  mNumGfx = 0;
}

// -------------------------------------------------------------------------- //

u32 TDynList2::fetchCmdIndex() {
  return mNumGfx;
}

// -------------------------------------------------------------------------- //

Gfx * TDynList2::getDL() const {
  if (mNumGfx >= mMaxNumGfx) {
    return nullptr;
  }

  switch (mFrameNum) {
    default:
    case 0: return &mGfxAry[mNumGfx]; break;
    case 1: return &mGfxAry2[mNumGfx]; break;
    case 2: return &mGfxAry3[mNumGfx]; break;
    case 3: return &mGfxAry4[mNumGfx]; break;
  }

}

// -------------------------------------------------------------------------- //

Gfx * TDynList2::pushDL() {
  Gfx * list = getDL();

  if (list != nullptr) {
    ++mNumGfx;
  }

  return list;
}

// -------------------------------------------------------------------------- //

Gfx * TDynList2::getHead() {
  switch (mFrameNum) {
    default:
    case 0: return mGfxAry; break;
    case 1: return mGfxAry2; break;
    case 2: return mGfxAry3; break;
    case 3: return mGfxAry4; break;
  }
}

// -------------------------------------------------------------------------- //

Mtx const &
TDynList2::getIdentityMtx() {
  static bool init { false };
  static Mtx mtx;

  if (!init) {
    guMtxIdent(&mtx);
    init = true;
  }

  return mtx;
}

// -------------------------------------------------------------------------- //
