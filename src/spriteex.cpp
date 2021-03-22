
#include <nusys.h>

#include "array.hpp"
#include "dynlist.hpp"
#include "spriteex.hpp"

// -------------------------------------------------------------------------- //

static Gfx sGfx2dInit[] = {
	gsDPPipeSync(),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPSetTextureLOD(G_TL_TILE),
	gsDPSetTextureLUT(G_TT_NONE),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetAlphaCompare(G_AC_THRESHOLD),
	gsDPSetBlendColor(0, 0, 0, 0x01),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
	gsSPEndDisplayList(),
};

// -------------------------------------------------------------------------- //

void TSpriteEx::load(
    Sprite const & sprite
) {
}

TVec2S const &
TSpriteEx::getPosition() const {
    return mPosition;
}

// -------------------------------------------------------------------------- //

void TSpriteEx::setPosition(
    TVec2S const & pos
) {
    mPosition = pos;
}

// -------------------------------------------------------------------------- //

TVec2F const &
TSpriteEx::getScale() const {
    return mScale;
}

// -------------------------------------------------------------------------- //

void TSpriteEx::setScale(
    TVec2F const & scale
) {
    mScale = scale;
}

// -------------------------------------------------------------------------- //

void TSpriteEx::draw() {
}

// -------------------------------------------------------------------------- //

void TSpriteEx::init(TDynList2 * dl) {
    gSPDisplayList(dl->pushDL(), sGfx2dInit);
}

// -------------------------------------------------------------------------- //

void TSpriteEx::finalize(TDynList2 * dl) {
    gSPDisplayList(dl->pushDL(), rdpinit_dl);
    gSPDisplayList(dl->pushDL(), rspinit_dl);
}

// -------------------------------------------------------------------------- //