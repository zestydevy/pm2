#include <nusys.h>

#include "title.hpp"
#include "scene.hpp"
#include "credits.hpp"
#include "util.hpp"
#include "game.hpp"
#include "app.hpp"
#include "pad.hpp"
#include "audio.hpp"

#include "segment.h"

#include "../models/ovl/sprites/sp_bg0.h"
#include "../models/ovl/sprites/sp_bg1.h"
#include "../models/ovl/sprites/sp_bg2.h"
#include "../models/ovl/sprites/sp_bg3.h"
#include "../models/ovl/sprites/sp_bg4.h"

extern "C" void bzero(void *, int);

static TTimer * sCrashTimer{nullptr};
static TTimer * sScrollTimer{nullptr};
static TTimer * sExitTimer{nullptr};
static bool sIsCredits{false};
static bool sIsLoop{false};
static EButton const sButtonCache[] = {UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, B, A};

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

    mMenuPad = new TPad(0);
    
    mBgStart = new TSprite;
    mBgLayer1 = new TSprite;
    mBgLayer2 = new TSprite;
    mBgLayer2Dup = new TSprite;
    mBgLayer3 = new TSprite;
    mBgLayer4 = new TSprite;

    mStartTimer = new TTimer;
    sCrashTimer = new TTimer;
    sScrollTimer = new TTimer;
    sExitTimer = new TTimer;
    mStartTimer->start(1);
    sExitTimer->start(110.0F);

    mCloud1X = 360 / 2;
    mCloud2X = -250 * -2;

    TAudio::playMusicMono(EBgm::BGM_INTRO);
    TAudio::fadeMusic(1.0F);

}

// -------------------------------------------------------------------------- //

void TTitleScene::update()
{
    mMenuPad->read();
    
    scrollCloudLayer();
    handleStart();
    waitForEnd();

    // can i press start now?
    if (mCanStart) {
        handleCheat();
        if (mMenuPad->isPressed(EButton::START)) {
            
            TAudio::fadeMusic(0.0f, 4.0f);
            auto handle = MusStartEffect(ESfxType::SFX_SELECT);
            MusHandleSetVolume(handle, 50);

            mStartState = 3;
            mStartAlpha = 0;
            mCanStart = false;
        }
    }

    TAudio::update();
}

// -------------------------------------------------------------------------- //

void TTitleScene::draw()
{
    // ...
}

// -------------------------------------------------------------------------- //

void TTitleScene::draw2D()
{   
    if (mStartState >= 4) return;
    
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

    mBgStart->load(bg0_sprite);
    mBgStart->setPosition(TVec2S{110, 178});
    mBgStart->setScale(TVec2F{1.0f, 1.0f});
    mBgStart->setColor({255,255,255,static_cast<u8>(mStartAlpha)});
    mBgStart->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
    mBgStart->draw();
}

// -------------------------------------------------------------------------- //

TScene * TTitleScene::exit()
{
    delete sCrashTimer;
    delete sScrollTimer;
    delete sExitTimer;

    if (sIsLoop) {
        sIsLoop = false;
        sIsCredits = false;
        return new TLogoScene { nullptr, mDynList };
    } else {
        return new TCreditsScene { "credits", mDynList, sIsCredits };
    }
}

// -------------------------------------------------------------------------- //

void TTitleScene::scrollCloudLayer()
{
    if (!sScrollTimer->update()) {
        ++mCloud1X;
        ++mCloud2X;
    } else {
        sScrollTimer->start(0.04F);
    }

    if (mCloud1X >= 480) {
        mCloud1X = -250;
    }

    if (mCloud2X >= 480) {
        mCloud2X = -250;
    }
}

// -------------------------------------------------------------------------- //

void TTitleScene::handleStart()
{
    float const delayTimer[3] = {1, 1, 1};
    auto game = TGame::getInstance();

    if (!mStartTimer->update()) {
        switch(mStartState)
        {
            case 0:
                mAlpha = TMath<s16>::clamp((mAlpha + 7), 0, 255);
                break;
            case 1:
                mAlpha = TMath<s16>::clamp((mAlpha + 7), 0, 255);
                mStartAlpha = TMath<s16>::clamp((mStartAlpha + 8), 0, 255);
                break;
            case 2:
                mCanStart = true;
                mStartAlpha = TMath<s16>::clamp((mStartAlpha - 8), 0, 255);
                break;
            case 3:
                mStartTimer->start(99.0f);
                //game->setClearColor(0,0,0,255);
                mAlpha = TMath<s16>::clamp((mAlpha - 4), 0, 255);
                if (mAlpha == 0) mStartState = 4;
                break;
            case 4:
                mAlpha = TMath<s16>::clamp((mAlpha + 6), 0, 255);
                game->setClearColor(-mAlpha,-mAlpha,-mAlpha,255);
                sCrashTimer->start(4);
                mStartState = 5;
                break;
            case 5:
                game->setClearColor(-mAlpha,-mAlpha,-mAlpha,255); 
                mAlpha = TMath<s16>::clamp((mAlpha + 6), 0, 255);
                if (sCrashTimer->update()) {
                    mStatus = ESceneState::EXITING;
                }
                break;
        }
    } else {
        mStartTimer->start(delayTimer[mStartState]);
        ++mStartState;
        if (mStartState > 2) mStartState = 1;
    }
}

// -------------------------------------------------------------------------- //

void TTitleScene::handleCheat()
{
    if (mMenuPad->isPressed(sButtonCache[mCheatState])){
        mCheatState++;

        if (mCheatState == sizeof(sButtonCache) / sizeof(EButton)){
            sIsCredits = true;
            
            TAudio::fadeMusic(0.0f, 8.0f);
            TAudio::playSound(ESfxType::SFX_SELECT);

            mStartState = 3;
            mStartAlpha = 0;
            mCanStart = false;
        }
    }
    else if (mMenuPad->isPressed((EButton)(A | B | UP | LEFT | RIGHT | DOWN)))
        mCheatState = 0;
}

// -------------------------------------------------------------------------- //

void TTitleScene::waitForEnd()
{
    if (sExitTimer->update()) {
        mCanStart = false;
        if (!sIsLoop) {
            sIsLoop = true;
            mStartState = 2;
            mStartTimer->start(2);
        }
    }

    if (sIsLoop && mStartTimer->update()) {
        mStatus = ESceneState::EXITING;
    }
}

// -------------------------------------------------------------------------- //