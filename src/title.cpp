#include <nusys.h>

#include "title.hpp"
#include "scene.hpp"
#include "util.hpp"
#include "game.hpp"
#include "app.hpp"

#include "segment.h"

#include "../models/ovl/sprites/sp_bg1.h"
#include "../models/ovl/sprites/sp_bg2.h"
#include "../models/ovl/sprites/sp_bg3.h"
#include "../models/ovl/sprites/sp_bg4.h"

extern "C" void bzero(void *, int);

// -------------------------------------------------------------------------- //

void TTitleScene::init()
{
    mStatus = ESceneState::RUNNING;

    // turn on screen
    //nuGfxDisplayOn();

    auto game = TGame::getInstance();
    game->setClearColor(255,255,255,1);
    
    // load logo into memory
    TUtil::toMemory(
        reinterpret_cast<void *>(_codeSegmentBssEnd), 
        _title_ovlSegmentRomStart, 
        _title_ovlSegmentRomEnd-_title_ovlSegmentRomStart
    );

    mBgLayer1 = new TSprite;
    mBgLayer2 = new TSprite;
    mBgLayer2Dup = new TSprite;
    mBgLayer3 = new TSprite;
    mBgLayer4 = new TSprite;

    mCloud1X = 360 / 2;
    mCloud2X = -250 * -2;

}

// -------------------------------------------------------------------------- //

void TTitleScene::update()
{
    scrollCloudLayer();

    mAlpha = TMath<s16>::clamp((mAlpha + 7), 0, 255);
}

// -------------------------------------------------------------------------- //

void TTitleScene::draw()
{
    // ...
}

// -------------------------------------------------------------------------- //

void TTitleScene::draw2D()
{   
    mBgLayer4->load(bg4_sprite);
    mBgLayer4->setPosition(TVec2S{0, 123});
    mBgLayer4->setScale(TVec2F{1.0f, 1.0f});
    mBgLayer4->setColor({255,255,255,static_cast<u8>(mAlpha)});
    mBgLayer4->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
    mBgLayer4->draw();

    mBgLayer3->load(bg3_sprite);
    mBgLayer3->setPosition(TVec2S{0, 0});
    mBgLayer3->setScale(TVec2F{1.0f, 1.0f});
    mBgLayer3->setColor({255,255,255,static_cast<u8>(mAlpha)});
    mBgLayer3->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
    mBgLayer3->draw();

    mBgLayer2->load(bg2_sprite);
    mBgLayer2->setPosition(TVec2S{mCloud1X, 60});
    mBgLayer2->setScale(TVec2F{1.0f, 1.0f});
    mBgLayer2->setColor({255,255,255,static_cast<u8>(mAlpha)});
    mBgLayer2->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
    mBgLayer2->draw();

    mBgLayer2Dup->load(bg2_sprite);
    mBgLayer2Dup->setPosition(TVec2S{mCloud2X, 60});
    mBgLayer2Dup->setScale(TVec2F{1.0f, 1.0f});
    mBgLayer2Dup->setColor({255,255,255,static_cast<u8>(mAlpha)});
    mBgLayer2Dup->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
    mBgLayer2Dup->draw();
    
    mBgLayer1->load(bg1_sprite);
    mBgLayer1->setPosition(TVec2S{0, 0});
    mBgLayer1->setScale(TVec2F{1.0f, 1.0f});
    mBgLayer1->setColor({255,255,255,static_cast<u8>(mAlpha)});
    mBgLayer1->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
    mBgLayer1->draw();
}

// -------------------------------------------------------------------------- //

TScene * TTitleScene::exit()
{
    return new TTitleScene { "title", mDynList };
}

// -------------------------------------------------------------------------- //

void TTitleScene::scrollCloudLayer()
{
    ++mCloud1X;
    ++mCloud2X;

    if (mCloud1X >= 480) {
        mCloud1X = -250;
    }

    if (mCloud2X >= 480) {
        mCloud2X = -250;
    }
}

// -------------------------------------------------------------------------- //