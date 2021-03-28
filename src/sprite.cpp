
#include <nusys.h>

#include "array.hpp"
#include "dynlist.hpp"
#include "sprite.hpp"

// -------------------------------------------------------------------------- //

TDynList2 * TSprite::sDynList { nullptr };

// -------------------------------------------------------------------------- //

void TSprite::load(
    Sprite const & sprite
) {
    mSpriteData.width = sprite.width;
    mSpriteData.height = sprite.height;
    mSpriteData.expx = sprite.expx;
    mSpriteData.expy = sprite.expy;
    mSpriteData.attr = sprite.attr;
    mSpriteData.zdepth = sprite.zdepth;
    mSpriteData.startTLUT = sprite.startTLUT;
    mSpriteData.nTLUT = sprite.nTLUT;
    mSpriteData.LUT = sprite.LUT;
    mSpriteData.istart = sprite.istart;
    mSpriteData.istep = sprite.istep;
    mSpriteData.nbitmaps = sprite.nbitmaps;
    mSpriteData.ndisplist = sprite.ndisplist;
    mSpriteData.bmheight = sprite.bmheight;
    mSpriteData.bmHreal = sprite.bmHreal;
    mSpriteData.bmfmt = sprite.bmfmt;
    mSpriteData.bmsiz = sprite.bmsiz;
    mSpriteData.bitmap = sprite.bitmap;
    mSpriteData.frac_s = sprite.frac_s;
    mSpriteData.frac_t = sprite.frac_t;
    preload(sprite);
}

// -------------------------------------------------------------------------- //

void TSprite::preload(
    Sprite const & sprite
) {
    auto size = (u32)sprite.ndisplist;

    if (size > mNumSpriteDL) {
        delete[] mSpriteDL;
        mSpriteDL = new Gfx[size];
        mSpriteData.rsp_dl = mSpriteDL;
        mNumSpriteDL = size;
    }
}

// -------------------------------------------------------------------------- //

u32 TSprite::getWidth() const {
    if (mSpriteData.width <= 0) {
        return 0;
    }

    return (u32)mSpriteData.width;
}

// -------------------------------------------------------------------------- //

u32 TSprite::getHeight() const {
    if (mSpriteData.height <= 0) {
        return 0;
    }

    return (u32)mSpriteData.height;
}

// -------------------------------------------------------------------------- //

TVec2S const &
TSprite::getPosition() const {
    return mPosition;
}

// -------------------------------------------------------------------------- //

void TSprite::setPosition(
    TVec2S const & pos
) {
    mPosition = pos;
}

// -------------------------------------------------------------------------- //

TVec2F const &
TSprite::getScale() const {
    return mScale;
}

// -------------------------------------------------------------------------- //

void TSprite::setScale(
    TVec2F const & scale
) {
    mScale = scale;
}

// -------------------------------------------------------------------------- //

TColor const &
TSprite::getColor() const {
    return mColor;
}

// -------------------------------------------------------------------------- //

void TSprite::setColor(
    TColor const & color
) {
    mColor = color;
}

// -------------------------------------------------------------------------- //

u32 TSprite::getAttributes() const {
    return mAttributes;
}

// -------------------------------------------------------------------------- //

void TSprite::setAttributes(
    u32 const attrib
) {
    mAttributes = attrib;
}

// -------------------------------------------------------------------------- //

void TSprite::setOnAttributes(
    u32 const mask
) {
    mAttributes |= mask;
}

// -------------------------------------------------------------------------- //

void TSprite::setOffAttributes(
    u32 const mask
) {
    mAttributes &= ~mask;
}

// -------------------------------------------------------------------------- //

void TSprite::draw() {
    if (mScale.x() <= 0.001F || mScale.y() <= 0.001F) {
        return;
    }

    if (mColor.a == 0) {
        return;
    }

    mSpriteData.rsp_dl_next = mSpriteData.rsp_dl;
    spMove(&mSpriteData, mPosition.x(), mPosition.y());
    spScale(&mSpriteData, mScale.x(), mScale.y());
    spColor(&mSpriteData, mColor.r, mColor.g, mColor.b, mColor.a);
    spSetAttribute(&mSpriteData, mAttributes);
    gSPDisplayList(sDynList->pushDL(), spDraw(&mSpriteData));
}

// -------------------------------------------------------------------------- //

void TSprite::init(TDynList2 * dl) {
    sDynList = dl;
    auto gfx = sDynList->getDL();
    spInit(&gfx);
}

// -------------------------------------------------------------------------- //

void TSprite::finalize() {
    auto dl = sDynList->getDL();
    gSPDisplayList(dl, rdpinit_dl);
    gSPDisplayList(dl, rspinit_dl);
}

// -------------------------------------------------------------------------- //