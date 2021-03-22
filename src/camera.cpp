#include <nusys.h>

#include "camera.hpp"
#include "pad.hpp"
#include "math.hpp"
#include "util.hpp"

// -------------------------------------------------------------------------- //

TCamera * TCamera::sCamera { nullptr };

// -------------------------------------------------------------------------- //

TCamera::TCamera(TDynList2 * list)
{
    mDynList = list;
    
    mPosMtx.identity();
    mRotMtx.identity();
    mScaleMtx.identity();

    mPosition.set(0.0f, 0.0f, 0.0f);
    mRotation.set(0.0f, 0.0f, 0.0f);
    mScale.set(1.0f, 1.0f, 1.0f);

    mDistance = 300.0f;

    mExternallyControlled = false;

    sCamera = this;
}

bool TCamera::checkVisible(const TVec3F & pos, float drawDistance = 100000000.0f){
    TVec3F dif = pos - sCamera->mOldPos;
    float sqrdist = dif.dot(dif);
    if (sqrdist > drawDistance)
        return false;
    if (sqrdist < 300.0f)
        return true;
    dif.normalize();
    return dif.dot(sCamera->mForward) > 0.6f;
}
bool TCamera::checkClipping(const TVec3F & pos, float radius, bool ignoreBehind){
    TVec3F dif = pos - sCamera->mOldPos;
    if (ignoreBehind && dif.dot(sCamera->mForward) < 0.0f)
        return false;
    return dif.getLength() <= radius;
}

void TCamera::jumpToTarget(){
    TVec3F dif = (*mTarget - mPosition);
    float pdist = dif.getLength();

    float x = mPosition.x();
    float y = mPosition.y();
    float z = mPosition.z();
    if (!mExternallyControlled){
        if (pdist > mDistance)      // Clamp camera distance between max and half max;
            pdist = mDistance;
        if (pdist < mDistance * 0.75f)
            pdist = mDistance * 0.75f;

        x = mTarget->x() - TSine::ssin(mAngle) * pdist;
        y = mTarget->y() + 20.00f;
        z = mTarget->z() - TSine::scos(mAngle) * pdist;
        mPosition.set(x, y, z);
    }
    mOldPos = mPosition;
}

void TCamera::drawWindow(float scale){
    //gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFWindowPosMtx),
	//      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_PUSH);
    //gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFWindowRotMtx),
	//      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    //    
    //gSPDisplayList(mDynList->pushDL(), window_HeldWindow_mesh);
//
    //gSPPopMatrix(mDynList->pushDL(), G_MTX_MODELVIEW);
}

void TCamera::update()
{    
    if (mDistance < 150.0f)
        mDistance = 150.0f;
    if (mDistance > 600.0f)
        mDistance = 600.0f;

    //get current target distance
    TVec3F dif = (*mTarget - mPosition);
    float pdist = dif.getLength();

    bool moveCamera = mPad->isHeld(EButton::C_LEFT) || mPad->isHeld(EButton::C_RIGHT) || mPad->isHeld(EButton::C_UP) || mPad->isHeld(EButton::C_DOWN);
    if (mPad->isHeld(EButton::C_LEFT))
        mAngle += 18000.0f * kInterval;
    if (mPad->isHeld(EButton::C_RIGHT))
        mAngle -= 18000.0f * kInterval;
    if (mPad->isHeld(EButton::C_UP))
        mDistance -= 210.0f * kInterval;
    if (mPad->isHeld(EButton::C_DOWN))
        mDistance += 210.0f * kInterval;

    float x = mPosition.x();
    float y = mPosition.y();
    float z = mPosition.z();
    if (!mExternallyControlled && (moveCamera || pdist > mDistance || pdist < mDistance * 0.75f)){
        if (pdist > mDistance)      // Clamp camera distance between max and half max;
            pdist = mDistance;
        if (pdist < mDistance * 0.75f)
            pdist = mDistance * 0.75f;

        if (!moveCamera)
            mAngle = TSine::atan2(dif.x(), dif.z());
        x = mTarget->x() - TSine::ssin(mAngle) * pdist;
        y = mTarget->y() + 20.00f;
        z = mTarget->z() - TSine::scos(mAngle) * pdist;
        mPosition.set(x, y, z);
    }

    //Don't let the camera clip in the ground
    mGroundFace = TCollision::findGroundBelow(mOldPos, 50.0f);
    if (mGroundFace != nullptr){
        float y = mGroundFace->calcYAt(mOldPos.xz()) + 50.0f;
        if (mOldPos.y() < y)
            mOldPos.y() = y;
    }

    mOldPos.lerpTime({x, y, z}, 0.0667f, kInterval);
    mViewMtx.lookAt(mOldPos * 0.1f, mTarget->xyz() * 0.1f, {0.0f,1.0f,0.0f});
    mRotMtx.rotateAxis(mRotation, 0);
    mScaleMtx.scale(mScale * 0.1f);

    mForward = *mTarget - mOldPos;
    mForward.normalize();
}

void TCamera::render()
{    
    if (mDistance < 150.0f)
        mDistance = 150.0f;
    if (mDistance > 600.0f)
        mDistance = 600.0f;

    guPerspective(&mProjectionMtx, &mPersp,
		      mFov, 320.0/240.0, 5, 8000, 1.0);
    guLookAtReflect(&mFViewMtx, mLookAtMtx,
		       50, 0, 400,
		       0, 0, 0,
		       0, 1, 0);
	
    gSPLookAt(mDynList->pushDL(), mLookAtMtx);
    gSPPerspNormalize(mDynList->pushDL(), mPersp);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mProjectionMtx),
		  G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFViewMtx),
		  G_MTX_PROJECTION|G_MTX_MUL|G_MTX_NOPUSH);

    //get current target distance
    TVec3F dif = (*mTarget - mPosition);
    float pdist = dif.getLength();

    bool moveCamera = mPad->isHeld(EButton::C_LEFT) || mPad->isHeld(EButton::C_RIGHT) || mPad->isHeld(EButton::C_UP) || mPad->isHeld(EButton::C_DOWN);
    if (mPad->isHeld(EButton::C_LEFT))
        mAngle += 18000.0f * 0.0f;
    if (mPad->isHeld(EButton::C_RIGHT))
        mAngle -= 18000.0f * 0.0f;
    if (mPad->isHeld(EButton::C_UP))
        mDistance -= 210.0f * 0.0f;
    if (mPad->isHeld(EButton::C_DOWN))
        mDistance += 210.0f * 0.0f;

    float x = mPosition.x();
    float y = mPosition.y();
    float z = mPosition.z();
    if (!mExternallyControlled && (moveCamera || pdist > mDistance || pdist < mDistance * 0.75f)){
        if (pdist > mDistance)      // Clamp camera distance between max and half max;
            pdist = mDistance;
        if (pdist < mDistance * 0.75f)
            pdist = mDistance * 0.75f;

        if (!moveCamera)
            mAngle = TSine::atan2(dif.x(), dif.z());
        x = mTarget->x() - TSine::ssin(mAngle) * pdist;
        y = mTarget->y() + 20.00f;
        z = mTarget->z() - TSine::scos(mAngle) * pdist;
        mPosition.set(x, y, z);
    }

    //Don't let the camera clip in the ground
    mGroundFace = TCollision::findGroundBelow(mOldPos, 50.0f);
    if (mGroundFace != nullptr){
        float y = mGroundFace->calcYAt(mOldPos.xz()) + 50.0f;
        if (mOldPos.y() < y)
            mOldPos.y() = y;
    }

    mOldPos.lerpTime({x, y, z}, 0.0667f, 0.0f);
    mViewMtx.lookAt(mOldPos * 0.1f, mTarget->xyz() * 0.1f, {0.0f,1.0f,0.0f});
    mRotMtx.rotateAxis(mRotation, 0);
    mScaleMtx.scale(mScale * 0.1f);

    TMtx44::floatToFixed(mPosMtx, mFPosMtx);
    TMtx44::floatToFixed(mRotMtx, mFRotMtx);
    TMtx44::floatToFixed(mScaleMtx, mFScaleMtx);
    TMtx44::floatToFixed(mViewMtx, mFViewMtx);
    
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFPosMtx),
	      G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFScaleMtx),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);
    gSPMatrix(mDynList->pushDL(), OS_K0_TO_PHYSICAL(&mFRotMtx),
	      G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

    mForward = *mTarget - mOldPos;
    mForward.normalize();


    // update facing mtx
    TMtx44 temp1, temp2, temp3;
    
    mWindowPosMtx.translate(mOldPos + (mForward * 1125.0f * 45.0f / mFov));

    temp1.rotateAxisX(0);
    temp2.rotateAxisY(TSine::atan2(-mForward.z(), mForward.x()));
    temp3.rotateAxisZ(TSine::acos(mForward.xz().getLength()) * (mForward.y() >= 0.0f ? 1.0f : -1.0f));
    TMtx44::concat(temp2, temp1, mWindowRotMtx);
    TMtx44::concat(mWindowRotMtx, temp3, mWindowRotMtx);

    TMtx44::floatToFixed(mWindowPosMtx, mFWindowPosMtx);
    TMtx44::floatToFixed(mWindowRotMtx, mFWindowRotMtx);
}

// -------------------------------------------------------------------------- //