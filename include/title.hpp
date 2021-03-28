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

class TTitleScene final
    : public TScene
{
    public:

    TTitleScene(char const * name, TDynList2 * list)
        : TScene(name, list) {};

    ~TTitleScene() = default;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void draw2D() override;
    virtual TScene * exit() override;

    private:

    void scrollCloudLayer();

    TSprite * mBgLayer1{nullptr};
    TSprite * mBgLayer2{nullptr};
    TSprite * mBgLayer2Dup{nullptr};
    TSprite * mBgLayer3{nullptr};
    TSprite * mBgLayer4{nullptr};
    s16 mAlpha{0};

    int mCloud1X{0};
    int mCloud2X{0};

};