#pragma once

#include <nusys.h>

#include "array.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "dynlist.hpp"
#include "sprite.hpp"
#include "pad.hpp"
#include "scenedata.h"
#include "staticobj.hpp"

// -------------------------------------------------------------------------- //

enum ESceneState : u32
{
    IDLE = 0,
    RUNNING,
    EXITING,
};

// -------------------------------------------------------------------------- //

class TScene
{
    public:

    inline TScene(char const * name, TDynList2 * list)
        : mName{name}, mDynList{list} {};

    virtual ~TScene() = default;
    
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void draw2D() = 0;
    virtual TScene * exit() = 0;

    void loadObjects(TSceneEntry const list[]);

    inline char const * getName() {return mName;}
    ESceneState getState() {return mStatus;}
    bool isInitialized();
    //virtual TPlayer * getPlayer() {return nullptr;}
    TDynList2 * getDynList() { return mDynList; }

    ESceneState mStatus{ESceneState::IDLE};
    TDynList2 * mDynList{nullptr};

    protected:

    char const * mName{nullptr};
    TArray<TObject *> mObjList{};
};

// -------------------------------------------------------------------------- //

class TLogoScene final
    : public TScene
{
    public:

    TLogoScene(char const * name, TDynList2 * list)
        : TScene(name, list) {};

    ~TLogoScene() = default;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void draw2D() override;
    virtual TScene * exit() override;

    private:

    void runBootTimer();
    void runLogoTimer();
    void runFadeLogo1();

    TSprite * mLogoSpr{nullptr};
    TSprite * mLogoNinSpr{nullptr};
    TTimer * mTimer{nullptr};
    TTimer * mShowTimer{nullptr};
    TTimer * mFadeTimer{nullptr};
    s32 mLogoX{0};
    s32 mLogoY{0};
    s16 mAlpha{0};
    s8 mLogoState{0};

};
// -------------------------------------------------------------------------- //

class TTestScene final
    : public TScene
{
    public:

    TTestScene(char const * name, TDynList2 * list)
        : TScene(name, list) {};

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void draw2D() override;
    virtual TScene * exit() override;
    //virtual TPlayer * getPlayer() override;

    void drawObjects(EDrawLayer layer);

    void clearCollisions(int start, int end);

    private:
    void loadCollision(const s16 collision[], TCollFace * dest, int offset = 0);

    TCamera * mCamera{nullptr};
    TPad * mPad{nullptr};
    //TPlayer * mBird{nullptr};
    //TFlockObj * mFlock{nullptr};

    int mCurrentLayer {0};

    TCollFace * mCollisionFaces;
    int mColL2Start;
    int mColL2End;
    int mColL3Start;
    int mColL3End;
    int mColL4Start;
    int mColL4End;

    int mCheatState{0};

//    TObject * mObjects[4];
    TObject * mSky{nullptr};
    
    TCollision * mCollision{nullptr};
};

// -------------------------------------------------------------------------- //