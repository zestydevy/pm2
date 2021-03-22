#include <nusys.h>

#include "staticobj.hpp"
#include "camera.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

static Gfx * gObjMeshList[] =
{
    nullptr,
};

// -------------------------------------------------------------------------- //

void TObject::setPosition(TVec3<f32> const & pos)
{
    mPosition = pos;
    mMtxNeedsUpdate = true;
}

void TObject::setRotation(TVec3<f32> const & rot)
{
    mRotation.set(rot.x(), rot.y(), rot.z());
    mMtxNeedsUpdate = true;
}

void TObject::setScale(TVec3<f32> const & scale)
{
    mScale = scale;
    mMtxNeedsUpdate = true;
}

void TObject::init()
{
    mInCamera = true;
    updateMtx();
    mMtxNeedsUpdate = true;
}

void TObject::increaseRadius(float threshold){

}

void TObject::updateMtx()
{
    if (!mInCamera){
        mMtxNeedsUpdate = true;
        return;
    }

    TMtx44 temp1, temp2, temp3, mPosMtx, mScaleMtx;
    
    mPosMtx.translate(mPosition);
    temp1.rotateAxisX(mRotation.x());
    temp2.rotateAxisY(mRotation.y());
    temp3.rotateAxisZ(mRotation.z());
    TMtx44::concat(temp2, temp1, mRotMtx);
    TMtx44::concat(mRotMtx, temp3, mRotMtx);
    mScaleMtx.scale(mScale);

    //Combine mtx
    TMtx44::concat(mPosMtx, mRotMtx, temp1);
    TMtx44::concat(temp1, mScaleMtx, temp2);

    TMtx44::floatToFixed(temp2, mFMtx);
    TMtx44::floatToFixed(mRotMtx, mFRotMtx);


    mMtxNeedsUpdate = false;
}

void TObject::update() {
    mInCamera = mAlwaysDraw || TCamera::checkVisible(mPosition, mDrawDistanceSquared);
    //mInCamera = true;
}

void TObject::draw()
{
    if (!mInCamera)
        return;

    if (mMtxNeedsUpdate)
        updateMtx();

    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFMtx),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH);
    //gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFRotMtx),
	//      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    //gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFScaleMtx),
	//      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
        
    if (mMesh != nullptr) {
        gSPDisplayList(mDynList->pushDL(), mMesh);
    }

    gSPPopMatrix(mDynList->pushDL(), G_MTX_MODELVIEW);
}

void TObject::drawRing()
{
}

// -------------------------------------------------------------------------- //

Gfx * TObject::getMeshGfx(
    EObjType const type
) {
    if (type >= EObjType::INVALID) {
        return nullptr;
    }

    return gObjMeshList[(u32)type];
}

const TObjectData & TObject::getNestObjectInfo(
    EObjType const type
) {
    if (type >= EObjType::INVALID) {
        return gObjectDataList[0];
    }

    return gObjectDataList[(u32)type];
}

// -------------------------------------------------------------------------- //

void TShadow::init() {
    TObject::init();
}

void TShadow::update() {
    TObject::update();
}

void TShadow::updateMtx()
{
    if (!mInCamera){
        mMtxNeedsUpdate = true;
        return;
    }

    TMtx44 temp1, temp2, temp3, mPosMtx, mScaleMtx, mScaleMtx2;
    
    mPosMtx.translate(mPosition);
    temp1.rotateAxisX(mRotation.x());
    temp2.rotateAxisY(mRotation.y());
    temp3.rotateAxisZ(mRotation.z());
    TMtx44::concat(temp2, temp1, mRotMtx);
    TMtx44::concat(mRotMtx, temp3, mRotMtx);
    mScaleMtx.scale(mScale);
    mScaleMtx2.scale({1.0f, 0.0f, 1.0f});

    //Combine mtx
    TMtx44::floatToFixed(mPosMtx, mFMtx);
    TMtx44::floatToFixed(mScaleMtx, mFScaleMtx1);
    TMtx44::floatToFixed(mScaleMtx2, mFScaleMtx2);
    TMtx44::floatToFixed(mRotMtx, mFRotMtx);

    mMtxNeedsUpdate = false;
}

void TShadow::draw() {
    if (!mInCamera)
        return;

    if (mMtxNeedsUpdate)
        updateMtx();

    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFMtx),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFRotMtx),
            G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFScaleMtx2),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    if (mParent != nullptr){
        gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mParent->getRotMtx()),
            G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    }
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFScaleMtx1),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
        
    if (mMesh != nullptr) {
        gSPDisplayList(mDynList->pushDL(), mMesh);
    }

    gSPPopMatrix(mDynList->pushDL(), G_MTX_MODELVIEW);
}