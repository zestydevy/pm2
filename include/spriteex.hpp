#pragma once

#include <nusys.h>

#include "array.hpp"
#include "dynlist.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

extern Gfx rspinit_dl[];
extern Gfx rdpinit_dl[];

// -------------------------------------------------------------------------- //

class TSpriteEx {

    public:

    TSpriteEx() = default;
    ~TSpriteEx() = default;

    void load(Sprite const &);

    u32 getWidth() const;
    u32 getHeight() const;

    TVec2S const & getPosition() const;
    void setPosition(TVec2S const &);

    TVec2F const & getScale() const;
    void setScale(TVec2F const &);

    void draw();

    // initializes S2DEX. call before utilizing sprites
    static void init(TDynList2 *);

    // call after sprites are finished rendering.
    static void finalize(TDynList2 *);

    private:

    Gfx * mMesh {nullptr};
    TVec2S mPosition { 0, 0 };
    TVec2F mScale { 1.0F, 1.0F };

};
