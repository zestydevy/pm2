#pragma once

#include <nusys.h>

#include "math.hpp"
#include "dynlist.hpp"

#include "../scene/object_info.h"

// -------------------------------------------------------------------------- //

enum EObjType : s16
{
    PLAYERSTART = -1,
    NONE = 0,
    DEBUG_CUBE,
    BALLOON,
    LUNCHTABLE,
    CHAIR,
    BASKET,
    BEER,
    NOODLE,
    MELON,
    APPLE,
    BALLOON_DEFLATED,
    BEEHIVE,
    BILLS,
    CARD,
    FLOWER,
    NECKLACE,
    RING,
    SOCCER,
    STICK,
    TABLE,
    CAT,
    CHICKEN,
    CRITIC,
    AVGN,
    WOMAN,
    CHILD,
    LOGO,
    STORE_E,
    STORE_B,
    STEPPINGSTONE,
    FENCE,
    TRUNK,
    LEAVES,
    BRANCH,
    ROOTS,
    MAILBOX,
    NEST,
    DEADTREE,
    BALDHOOKTREE,
    BALDTREE,
    TIRE,
    TOASTER,
    PLANK,
    TRASHCAN,
    TRASHCAN2,
    POT,
    BUSH,
    LAMP,
    CAR,
    GOAL,
    INVALID
};

enum EDrawLayer : s16
{
    PREWINDOW,
    POSTWINDOW,
    LAYERNONE
};

// -------------------------------------------------------------------------- //

class TObject
{
    public:

    TObject(TDynList2 * list)
        : mDynList{list} {}
    virtual ~TObject() = default;

    void setPosition(TVec3<f32> const & pos);
    void setRotation(TVec3<f32> const & rot);
    void setScale(TVec3<f32> const & scale);
    inline void setMesh(Gfx * mesh, Gfx * initializer = nullptr) { mMesh = mesh; mInitializer = initializer; }
    const Gfx * getGraphicsInitializer(){ return mInitializer; };

    TVec3<f32> const & getPosition() {return mPosition;}
    TVec3<s16> const & getRotation() {return mRotation;}
    TVec3<f32> const & getScale() {return mScale;}

    const Mtx & getDrawMtx() { return mFMtx; }
    const Mtx & getRotMtx() { return mFRotMtx; }

    virtual void updateMtx();

    virtual void init();
    virtual void update();
    virtual void draw();
    virtual void drawRing();

    virtual void increaseRadius(float threshold);

    static Gfx * getMeshGfx(EObjType type);
    static const TObjectData & getNestObjectInfo(EObjType type);

    bool mAlwaysDraw{false};

    EDrawLayer mDrawLayer {EDrawLayer::PREWINDOW};

    protected:
    bool mInCamera{false};
    bool mMtxNeedsUpdate{true};

    TVec3<f32> mPosition{};
    TVec3<s16> mRotation{};
    TVec3<f32> mScale{};
    
    TMtx44 mRotMtx{};

    // 1 matrix push call per object. 2 when the object is being carried.
    Mtx mFMtx{};
    Mtx mFRotMtx{}; //Only used for storing rotation matrix when carrying an object
    
    u32 mTag{0};
    Gfx * mInitializer{nullptr};
    Gfx * mMesh{nullptr};
    TDynList2 * mDynList{nullptr};

    float mDrawDistanceSquared{100000000.0f};
};

// -------------------------------------------------------------------------- //

class TShadow : public TObject
{
    public:

    TShadow(TDynList2 * dl) :
        TObject { dl }
    {
    }

    virtual ~TShadow() = default;

    virtual void updateMtx() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setParent(TObject * obj) {mParent = obj;}

    protected:
    Mtx mFScaleMtx1{};
    Mtx mFScaleMtx2{};

    TObject * mParent {nullptr};
};

// -------------------------------------------------------------------------- //