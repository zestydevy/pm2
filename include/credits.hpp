#pragma once

#include <nusys.h>

#include "scene.hpp"
#include "array.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "dynlist.hpp"
#include "sprite.hpp"
#include "pad.hpp"
#include "scenedata.h"
#include "staticobj.hpp"

// -------------------------------------------------------------------------- //

class TCreditsScene final
    : public TScene
{
    public:

    TCreditsScene(char const * name, TDynList2 * list, bool credits)
        : TScene(name, list) {mIsCredits = credits;}

    ~TCreditsScene() = default;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void draw2D() override;
    virtual TScene * exit() override;

    private:

    bool mIsCredits{false};

    TSprite * mCreditsBg{nullptr};

};