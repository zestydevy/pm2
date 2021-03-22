#pragma once

#include <nusys.h>

#include "animator.hpp"
#include "camera.hpp"
#include "collider.hpp"
#include "collision.hpp"
#include "dynlist.hpp"
#include "math.hpp"
#include "pad.hpp"
#include "staticobj.hpp"

#include "../models/ovl/bird/model_bird.h"
#include "../models/ovl/bird_eagle/model_eagle.h"

enum playerstate_t : u16 {
    PLAYERSTATE_FALLING,    //Falling
    PLAYERSTATE_IDLE,       //Idle
    PLAYERSTATE_WALKING,    //Walking on ground
    PLAYERSTATE_FLAPPING,   //Precision slow fly controls
    PLAYERSTATE_FLYING,     //Plane fly controls
    PLAYERSTATE_UTURN,
    PLAYERSTATE_STUNNED,     //Hit a wall while flying
};

enum gameplaystate_t : u16 {
    PLAYERGAMESTATE_FREEFLIGHT,
    PLAYERGAMESTATE_COUNTDOWN,
    PLAYERGAMESTATE_FLIGHT,
    PLAYERGAMESTATE_FINISHED
};


enum playeranim_t : u16 {
    ANIM_IDLE,
    ANIM_IDLEPREEN,
    ANIM_WALK,
    ANIM_GLIDE,
    ANIM_GLIDEFAST,
    //ANIM_GLIDECAW,
    ANIM_GLIDECRASH,
    ANIM_GLIDEFLAP,
    ANIM_FLAP,
    ANIM_IDLEFALL,
    ANIM_IDLECAW,
    ANIM_GLIDEUTURN,
    ANIM_COUNT
};

// -------------------------------------------------------------------------- //

class TPlayer;

extern TPlayer * gPlayer;

// -------------------------------------------------------------------------- //

class TPlayer :
    public TObject,
    public TSphereCollider
{

    public:

    TPlayer(TDynList2 * dl) :
        TObject { dl }
    {
        gPlayer = this;
    }

    virtual ~TPlayer() = default;

    void setPad(TPad * pad) {
        mPad = pad;
    }

    void setCamera(TCamera * camera) {
        mCamera = camera;
        mCamera->setTarget(&mCameraTarget);
    }

    TCollFace const * getGroundFace() const {
        return mGroundFace;
    }

    TVec3F getVelocity() {
        return mDirection * mSpeed;
    }

    bool canPickupObjects() { return mState == playerstate_t::PLAYERSTATE_FLYING; }

    virtual void updateMtx() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void hitObject(TVec3F point, EObjType type);
    void collectObject(EObjType type);
    void passthroughObject(EObjType type);

    const TCollFace * mClosestFace;
    const TCollFace * mClosestFaceCarry;

    const TVec3<f32> & getHeldPosition(int idx = 0) { return mHeldPos[idx]; }
    const TVec3<f32> & getHeldVelocity(int idx = 0) { return mHeldVel[idx]; }
    float getHeldChainLength();
    TVec3<f32> mHeldPos[32];
    TVec3<f32> mHeldVel[32];

    void startFreeFlight() { mGameState = gameplaystate_t::PLAYERGAMESTATE_FREEFLIGHT; }

    void setAnimation(int length, playeranim_t anim, bool loop = true, float timescale = 0.25f);

    protected:
    void startFlying();
    void startIdle();
    void checkLateralCollision();
    void checkMeshCollision(const TCollFace * face, float radius);

    bool canMove() { return mGameState == gameplaystate_t::PLAYERGAMESTATE_FLIGHT || mGameState == gameplaystate_t::PLAYERGAMESTATE_FREEFLIGHT; }

    /* sfx */
    void playCawSFX();
    void playStumbleSFX();
    void playCrashSFX();
    void playBalloonPopSFX();
    void playCatSFX();
    void playChickenSFX();
    void playLeavesShuffleSFX();
    void playCriticSFX();

    TAnimator * mAnim{nullptr};
    TPad * mPad{nullptr};

    TShadow * mShadow{nullptr};

    TCamera * mCamera;
    TVec3<f32> mCameraTarget{};

    f32 mSpeed;
    bool mIsGround{false};
    TVec3<f32> mDirection{};
    TVec3<f32> mLastDirection{};
    TVec3F mCenter{};
    f32 mVelocity{0.0f};

    TCollFace const * mGroundFace { nullptr };

    /* Mesh Info */
    Vtx* mMeshes[4] = {bird_Bird_mesh_vtx_0, bird_Bird_mesh_vtx_1, bird_Bird_mesh_vtx_2, bird_Bird_mesh_vtx_3};
    int mMeshSizes[4] = {265, 75, 59, 8};
    Vtx* mMeshesEagle[4] = {bird_eagle_Bird_mesh_vtx_0, bird_eagle_Bird_mesh_vtx_1, bird_eagle_Bird_mesh_vtx_2, bird_eagle_Bird_mesh_vtx_3};
    int mMeshSizesEagle[4] = {259, 75, 59, 8};

    /* Animations Bird */
    Vtx** mAnim_Idle[4] = {bird_Bird_Idle_0, bird_Bird_Idle_1, bird_Bird_Idle_2, bird_Bird_Idle_3};                                 //Idle standing
    Vtx** mAnim_IdlePreen[4] = {bird_Bird_IdlePreen_0, bird_Bird_IdlePreen_1, bird_Bird_IdlePreen_2, bird_Bird_IdlePreen_3};        //Idle preen
    Vtx** mAnim_Walk[4] = {bird_Bird_Walk_0, bird_Bird_Walk_1, bird_Bird_Walk_2, bird_Bird_Walk_3};                                 //Walking
    Vtx** mAnim_Glide[4] = {bird_Bird_Glide_0, bird_Bird_Glide_1, bird_Bird_Glide_2, bird_Bird_Glide_3};                            //Gliding
    Vtx** mAnim_GlideFast[4] = {bird_Bird_GlideFast_0, bird_Bird_GlideFast_1, bird_Bird_GlideFast_2, bird_Bird_GlideFast_3};        //Gliding Fast
    // Vtx** mAnim_GlideCaw[4] = {bird_Bird_GlideCaw_0, bird_Bird_GlideCaw_1, bird_Bird_GlideCaw_2, bird_Bird_GlideCaw_3};             //Glide caw
    Vtx** mAnim_GlideCrash[4] = {bird_Bird_GlideCrash_0, bird_Bird_GlideCrash_1, bird_Bird_GlideCrash_2, bird_Bird_GlideCrash_3};   //Stunned animation 
    Vtx** mAnim_GlideFlap[4] = {bird_Bird_GlideFlap_0, bird_Bird_GlideFlap_1, bird_Bird_GlideFlap_2, bird_Bird_GlideFlap_3};        //Flapping wings while gliding
    Vtx** mAnim_Flap[4] = {bird_Bird_FlyFlap_0, bird_Bird_FlyFlap_1, bird_Bird_FlyFlap_2, bird_Bird_FlyFlap_3};                     //Flapping wings while stationary
    Vtx** mAnim_IdleFall[4] = {bird_Bird_IdleFall_0, bird_Bird_IdleFall_1, bird_Bird_IdleFall_2, bird_Bird_IdleFall_3};             //Falling
    Vtx** mAnim_IdleCaw[4] = {bird_Bird_IdleCaw_0, bird_Bird_IdleCaw_1, bird_Bird_IdleCaw_2, bird_Bird_IdleCaw_3};                  //Cawing while stationary
    Vtx** mAnim_GlideUTurn[4] = {bird_Bird_GlideUTurn_0, bird_Bird_GlideUTurn_1, bird_Bird_GlideUTurn_2, bird_Bird_GlideUTurn_3};   //UTurn

    /* Animations Eagle */
    Vtx** mAnim_Eagle_Idle[4] = {bird_eagle_Bird_Idle_0, bird_eagle_Bird_Idle_1, bird_eagle_Bird_Idle_2, bird_eagle_Bird_Idle_3};                                 //Idle standing
    Vtx** mAnim_Eagle_IdlePreen[4] = {bird_eagle_Bird_IdlePreen_0, bird_eagle_Bird_IdlePreen_1, bird_eagle_Bird_IdlePreen_2, bird_eagle_Bird_IdlePreen_3};        //Idle preen
    Vtx** mAnim_Eagle_Walk[4] = {bird_eagle_Bird_Walk_0, bird_eagle_Bird_Walk_1, bird_eagle_Bird_Walk_2, bird_eagle_Bird_Walk_3};                                 //Walking
    Vtx** mAnim_Eagle_Glide[4] = {bird_eagle_Bird_Glide_0, bird_eagle_Bird_Glide_1, bird_eagle_Bird_Glide_2, bird_eagle_Bird_Glide_3};                            //Gliding
    Vtx** mAnim_Eagle_GlideFast[4] = {bird_eagle_Bird_GlideFast_0, bird_eagle_Bird_GlideFast_1, bird_eagle_Bird_GlideFast_2, bird_eagle_Bird_GlideFast_3};        //Gliding Fast
    // Vtx** mAnim_Eagle_GlideCaw[4] = {bird_eagle_Bird_GlideCaw_0, bird_eagle_Bird_GlideCaw_1, bird_eagle_Bird_GlideCaw_2, bird_eagle_Bird_GlideCaw_3};             //Glide caw
    Vtx** mAnim_Eagle_GlideCrash[4] = {bird_eagle_Bird_GlideCrash_0, bird_eagle_Bird_GlideCrash_1, bird_eagle_Bird_GlideCrash_2, bird_eagle_Bird_GlideCrash_3};   //Stunned animation 
    Vtx** mAnim_Eagle_GlideFlap[4] = {bird_eagle_Bird_GlideFlap_0, bird_eagle_Bird_GlideFlap_1, bird_eagle_Bird_GlideFlap_2, bird_eagle_Bird_GlideFlap_3};        //Flapping wings while gliding
    Vtx** mAnim_Eagle_Flap[4] = {bird_eagle_Bird_FlyFlap_0, bird_eagle_Bird_FlyFlap_1, bird_eagle_Bird_FlyFlap_2, bird_eagle_Bird_FlyFlap_3};                     //Flapping wings while stationary
    Vtx** mAnim_Eagle_IdleFall[4] = {bird_eagle_Bird_IdleFall_0, bird_eagle_Bird_IdleFall_1, bird_eagle_Bird_IdleFall_2, bird_eagle_Bird_IdleFall_3};             //Falling
    Vtx** mAnim_Eagle_IdleCaw[4] = {bird_eagle_Bird_IdleCaw_0, bird_eagle_Bird_IdleCaw_1, bird_eagle_Bird_IdleCaw_2, bird_eagle_Bird_IdleCaw_3};                  //Cawing while stationary
    Vtx** mAnim_Eagle_GlideUTurn[4] = {bird_eagle_Bird_GlideUTurn_0, bird_eagle_Bird_GlideUTurn_1, bird_eagle_Bird_GlideUTurn_2, bird_eagle_Bird_GlideUTurn_3};   //UTurn

    /* Animation sets */
    Vtx*** mAnimSet_Normal[ANIM_COUNT] = {mAnim_Idle, mAnim_IdlePreen, mAnim_Walk, mAnim_Glide, mAnim_GlideFast, mAnim_GlideCrash, mAnim_GlideFlap, mAnim_Flap, mAnim_IdleFall, mAnim_IdleCaw, mAnim_GlideUTurn};
    Vtx*** mAnimSet_Eagle[ANIM_COUNT] = {mAnim_Eagle_Idle, mAnim_Eagle_IdlePreen, mAnim_Eagle_Walk, mAnim_Eagle_Glide, mAnim_Eagle_GlideFast, mAnim_Eagle_GlideCrash, mAnim_Eagle_GlideFlap, mAnim_Eagle_Flap, mAnim_Eagle_IdleFall, mAnim_Eagle_IdleCaw, mAnim_Eagle_GlideUTurn};
    int* mAnimLength_Normal[ANIM_COUNT] = {&bird_Bird_Idle_Length, &bird_Bird_IdlePreen_Length, &bird_Bird_Walk_Length, &bird_Bird_Glide_Length, &bird_Bird_GlideFast_Length, &bird_Bird_GlideCrash_Length, &bird_Bird_GlideFlap_Length, &bird_Bird_FlyFlap_Length, &bird_Bird_IdleFall_Length, &bird_Bird_IdleCaw_Length, &bird_Bird_GlideUTurn_Length};
    int* mAnimLength_Eagle[ANIM_COUNT] = {&bird_eagle_Bird_Idle_Length, &bird_eagle_Bird_IdlePreen_Length, &bird_eagle_Bird_Walk_Length, &bird_eagle_Bird_Glide_Length, &bird_eagle_Bird_GlideFast_Length, &bird_eagle_Bird_GlideCrash_Length, &bird_eagle_Bird_GlideFlap_Length, &bird_eagle_Bird_FlyFlap_Length, &bird_eagle_Bird_IdleFall_Length, &bird_eagle_Bird_IdleCaw_Length, &bird_eagle_Bird_GlideUTurn_Length};

    bool mCawing{false};
    bool mFlappingWings{false};
    bool mSlowingDown{false};
    bool mGoingFast{false};
    
    float mPitchModifier {0.0f};

    float mIdleTimer{0.0f};
    float mStutterTimer{0.0f};
    float mCawTimer {0.0f};
    float mFlapTimer{0};

    s16 mBankAngle {0};

    s16 mEndCameraAngle {0};
    float mEndCameraDistance {0.0f};
    float mEndCameraTimer {0.0f};
    playerstate_t mState{playerstate_t::PLAYERSTATE_IDLE};
    gameplaystate_t mGameState{gameplaystate_t::PLAYERGAMESTATE_COUNTDOWN};

    virtual void onCollide(TCollider *) override;
    void moveCameraRelative(TVec3F & move, TVec3F & forward, TVec3F & right, float multiplier = 1.0f);

};

// -------------------------------------------------------------------------- //