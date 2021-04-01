#include <nusys.h>

#include "credits.hpp"

#include "scene.hpp"
#include "util.hpp"
#include "game.hpp"
#include "app.hpp"
#include "pad.hpp"
#include "audio.hpp"

#include "../models/ovl/sprites/sp_crash.h"
#include "../models/ovl/sprites/sp_credits.h"

#include "segment.h"

// -------------------------------------------------------------------------- //

void TCreditsScene::init()
{
    mStatus = ESceneState::RUNNING;

    
    if (mIsCredits) {
        TAudio::playSound(ESfxType::SFX_KARASU);
    }

    // load logo into memory
        TUtil::toMemory(
            reinterpret_cast<void *>(_codeSegmentBssEnd), 
            _crash_ovlSegmentRomStart, 
            _crash_ovlSegmentRomEnd-_crash_ovlSegmentRomStart
        );
    
    mCreditsBg = new TSprite;
}

// -------------------------------------------------------------------------- //

void TCreditsScene::update()
{
    TAudio::update();
}

// -------------------------------------------------------------------------- //

void TCreditsScene::draw()
{
    // ...
}

// -------------------------------------------------------------------------- //

void TCreditsScene::draw2D()
{   
    if (mIsCredits)
    {
        mCreditsBg->load(credits_sprite);
        mCreditsBg->setPosition(TVec2S{0, 0});
        mCreditsBg->setScale(TVec2F{1.0f, 1.0f});
        mCreditsBg->setColor({255,255,255,255});
        mCreditsBg->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
        mCreditsBg->draw();
    } else
    {
        mCreditsBg->load(crash_sprite);
        mCreditsBg->setPosition(TVec2S{0, 0});
        mCreditsBg->setScale(TVec2F{1.0f, 1.0f});
        mCreditsBg->setColor({255,255,255,255});
        mCreditsBg->setAttributes(SP_Z | SP_OVERLAP | SP_TRANSPARENT);
        mCreditsBg->draw();
    }
}

// -------------------------------------------------------------------------- //

TScene * TCreditsScene::exit()
{
    return nullptr;
}

// -------------------------------------------------------------------------- //