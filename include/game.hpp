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

    void setCurrentScene(TScene * scene) { mScene = scene; }
    TScene * getCurrentScene() {return mScene; }

    static void testRender(u32 taskNum);

    private:

    float mTheta{0.0f};

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
