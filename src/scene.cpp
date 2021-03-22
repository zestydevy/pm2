
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

#include "../scene/object_info.h"
#include "../models/ovl/sprites/sprite_logo.h"

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

    // scene start timer
    mShowTimer->start(1);

    // move logo off-screen
    mLogoX = 512;
}

// -------------------------------------------------------------------------- //

void TLogoScene::update()
{
    // wait two seconds to boot
    if (mShowTimer != nullptr) {
        runBootTimer();
    }
}

// -------------------------------------------------------------------------- //

void TLogoScene::draw()
{
    // ...
}

void TLogoScene::draw2D()
{

    TSprite logo = TSprite();
    logo.load(logo_sprite);
    logo.setPosition(TVec2S{mLogoX, mLogoY});
    logo.setScale(TVec2F{1.0f, 1.0f});
    logo.setColor({255,255,255,mAlpha});
    logo.setAttributes(SP_FRACPOS | SP_TRANSPARENT);
    logo.draw();
}

TScene * TLogoScene::exit()
{
    // turn off screen
    //nuGfxDisplayOff();

    return new TLogoScene { "logo", mDynList };

}

// -------------------------------------------------------------------------- //

void TLogoScene::runBootTimer()
{
    if (mShowTimer->update()) {
        delete mShowTimer;
        mShowTimer = nullptr;
        mLogoX = 40;        // move logo on screen
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