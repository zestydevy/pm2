
#include <nusys.h>

#include "audio.hpp"
#include "collision.h"
#include "graphic.h"
#include "heap.hpp"
#include "scene.hpp"
#include "scenedata.h"
#include "segment.h"
#include "sprite.hpp"
#include "staticobj.hpp"
#include "util.hpp"
#include "math.hpp"
#include "game.hpp"

#include "../scene/object_info.h"
#include "../models/ovl/sprites/sp_logo1.h"
#include "../models/ovl/sprites/sp_logo2.h"

// -------------------------------------------------------------------------- //

extern Gfx rdpinit_spr_dl[];
extern Gfx rdpinit_dl[];
extern Gfx rspinit_dl[];

// -------------------------------------------------------------------------- //

Lights2 litc2 = gdSPDefLights2(
    0x5, 0x5, 0x5, /* ambient color */
    100, 100, 0,   /* light color */
    -32, -64, -32, /* normal */
    50, 50, 0,     /* light color */
    15, 30, 120    /* normal */
);

Gfx letters_setup_dl[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPSetGeometryMode(
        G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH |
        G_CULL_BACK | G_LIGHTING
    ), gsSPSetLights2(litc2),
    gsDPSetCombineMode (G_CC_SHADE, G_CC_SHADE),
    gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

// -------------------------------------------------------------------------- //

bool TScene::isInitialized()
{
    return (mStatus == ESceneState::RUNNING);
}

// -------------------------------------------------------------------------- //

void TScene::loadObjects(TSceneEntry const list[])
{
    s32 size = list[0].id;
    list = reinterpret_cast<const TSceneEntry *>(&list[1]);
    // mObjList.reserve((u32)size);

    for(int i = 0; i < size; ++i) {
        
        if (list[i].id >= EObjType::INVALID) {
            // wtf kind of model are you tryin to load
            return;
        }

        TObject * obj = nullptr;
        auto type = (EObjType)list[i].id;

        switch(list[i].id) {
            case EObjType::PLAYERSTART:
                //gPlayer->setPosition({list[i].positionX, list[i].positionY, list[i].positionZ});
                break;
            case EObjType::DEBUG_CUBE:
                obj = new TObject(mDynList);
                obj->setMesh(TObject::getMeshGfx(type));
                break;
            default: 
                break;
        }

        if (obj == nullptr) {
            continue;
        }

        obj->setPosition({list[i].positionX, list[i].positionY, list[i].positionZ});
        obj->setRotation({TSine::fromDeg(list[i].rotationX), TSine::fromDeg(list[i].rotationY), TSine::fromDeg(list[i].rotationZ)});
        obj->setScale({list[i].scaleX, list[i].scaleY, list[i].scaleZ});
        obj->init();

        mObjList.push(obj);
    }
}

// -------------------------------------------------------------------------- //

void TLogoScene::init()
{
    mStatus = ESceneState::RUNNING;

    // turn on screen
    //nuGfxDisplayOn();

    // load logo into memory
    TUtil::toMemory(
        reinterpret_cast<void *>(_codeSegmentBssEnd), 
        _logo_ovlSegmentRomStart, 
        _logo_ovlSegmentRomEnd-_logo_ovlSegmentRomStart
    );

    mShowTimer = new TTimer;
    mFadeTimer = new TTimer;

    // scene start timer
    mShowTimer->start(2);

    // move logo off-screen
    mLogoX = 512;

    mLogoSpr = new TSprite;
    mLogoNinSpr = new TSprite;
}

// -------------------------------------------------------------------------- //

void TLogoScene::update()
{
    // wait two seconds to boot
    if (mShowTimer != nullptr) {
        runBootTimer();
    } else {
        runFadeLogo1();
    }
}

// -------------------------------------------------------------------------- //

void TLogoScene::draw()
{
    // ...
}

void TLogoScene::draw2D()
{
    switch(mLogoState)
    {
        case 0:
        case 1:
            mLogoSpr->load(logo1_sprite);
            mLogoSpr->setPosition(TVec2S{mLogoX, mLogoY});
            mLogoSpr->setScale(TVec2F{1.0f, 1.0f});
            mLogoSpr->setColor({255,255,255,static_cast<u8>(mAlpha)});
            mLogoSpr->setAttributes(SP_FRACPOS | SP_TRANSPARENT);
            mLogoSpr->draw();
            break;
        case 2:
        case 3:
            mLogoSpr->load(logo2_sprite);
            mLogoSpr->setPosition(TVec2S{mLogoX, mLogoY});
            mLogoSpr->setScale(TVec2F{1.0f, 1.0f});
            mLogoSpr->setColor({255,255,255,static_cast<u8>(mAlpha)});
            mLogoSpr->setAttributes(SP_FRACPOS | SP_TRANSPARENT);
            mLogoSpr->draw();
    }


}

TScene * TLogoScene::exit()
{
    // turn off screen
    //nuGfxDisplayOff();

    delete mLogoSpr;
    delete mLogoNinSpr;
    delete mFadeTimer;
    return new TLogoScene { "logo", mDynList };

}

// -------------------------------------------------------------------------- //

void TLogoScene::runBootTimer()
{
    if (mShowTimer->update()) {
        delete mShowTimer;
        mShowTimer = nullptr;
        mLogoX = 0;        // move logo on screen
        mFadeTimer->start(8);
        //TAudio::playSound(ESfxType::SFX_CAW);
    }
}

// -------------------------------------------------------------------------- //

void TLogoScene::runLogoTimer()
{
    if (mTimer->update()) {
        mAlpha -= 4;
        if (mAlpha <= 4) {
            mAlpha = 0;

            delete mTimer;
            mTimer = nullptr;

            mStatus = ESceneState::EXITING;     // exit scene
        }
    }
}

// -------------------------------------------------------------------------- //

void TLogoScene::runFadeLogo1()
{
    const float delayList[5] = {5, 5, 8, 8, 99999};
    auto game = TGame::getInstance();

    if (!mFadeTimer->update()) {
        switch(mLogoState)
        {
            case 0:
                mAlpha = TMath<s16>::clamp((mAlpha + 2), 0, 255);
                break;
            case 1:
                game->setClearColor(255,255,255,1);
                mAlpha = TMath<s16>::clamp((mAlpha - 2), 0, 255);
                break;
            case 2:
                mAlpha = TMath<s16>::clamp((mAlpha + 2), 0, 255);
                break;
            case 3:
                mAlpha = TMath<s16>::clamp((mAlpha - 2), 0, 255);
                break;
            case 4:
                mStatus = ESceneState::EXITING;
                break;
        }
    } else {
        ++mLogoState;
        mFadeTimer->start(delayList[mLogoState]);
    }
}