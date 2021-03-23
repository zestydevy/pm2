#pragma once

#include <nusys.h>

//#include "dynlist.hpp"
#include "scene.hpp"
#include "array.hpp"
#include "dynlist.hpp"
#include "camera.hpp"

#include "animator.hpp"

// ========================================================================== //

class TGame
{
    public:

    TGame();
    ~TGame() = delete;

    static inline TGame * getInstance()
    {
        return sGameInstance;
    }
    
    void init();
    void update();
    void draw();
    void exit();

    void initRcpSegment();
    void initZBuffer();
    void initFrameBuffer();
    void inline setClearColor(u8 r, u8 g, u8 b, u8 a) 
    {mClearColor = (TColor::pack(r,g,b,a)) << 16 | (TColor::pack(r,g,b,a));}

    void setCurrentScene(TScene * scene) { mScene = scene; }
    TScene * getCurrentScene() {return mScene; }

    static void testRender(u32 taskNum);

    private:

    float mTheta{0.0f};
    u32 mClearColor{static_cast<u32>((TColor::pack(0,0,0,1)) << 16 | (TColor::pack(0,0,0,1)))};

    TBlockHeap * mBlockHeap { nullptr };
    TScene * mScene { nullptr };
    TDynList2 * mDynList{nullptr};
    TDynList2 * mClearDL{nullptr};
    TCamera * mCamera{nullptr};

    protected:
    int mCurrentFrame = 0;

    TAnimator * mBirdAnim;

    static TGame * sGameInstance;
};

// -------------------------------------------------------------------------- //
