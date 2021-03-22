
#include <nusys.h>

#include "util.hpp"

// -------------------------------------------------------------------------- //

void TUtil::toMemory(void * dest, void * src, s32 size)
{
    // transfer size can't be an odd number
    if (size & 1) {
        ++size;
    }
    
    nuPiReadRom(reinterpret_cast<u32>(src), dest, size);

    // invalidate data cache
    osInvalDCache(dest, size);
}

// -------------------------------------------------------------------------- //

void TTimer::start(float seconds) {
  mTime = seconds;
}

// -------------------------------------------------------------------------- //

bool TTimer::update() {
  if (mTime == 0.0F) {
    return true;
  }

  mTime -= kInterval; // 30 Hz

  if (mTime < 0.0F) {
    mTime = 0.0F;
  }

  return false;
}

// -------------------------------------------------------------------------- //

#define SEC_PER_MIN  60
#define MS_PER_SEC   1000

float TTimer::get(u32 * min, u32 * sec, u32 * ms) const {
  if (min != nullptr) {
    *min = (u32)(mTime / SEC_PER_MIN);
  }

  if (sec != nullptr) {
    *sec = ((u32)mTime % SEC_PER_MIN);
  }

  if (ms != nullptr) {
    *ms = ((u32)(mTime * MS_PER_SEC) % MS_PER_SEC);
  }

  return mTime;
}

#undef MS_PER_SEC
#undef SEC_PER_MIN

// -------------------------------------------------------------------------- //
