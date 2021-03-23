#pragma once

#include <nusys.h>

#include "array.hpp"
#include "dynlist.hpp"
#include "math.hpp"

// -------------------------------------------------------------------------- //

struct TColor
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    static inline u16 pack(u8 r, u8 g, u8 b, u8 a)
    {
        return static_cast<u16>(((((r)<<8) & 0xf800) | (((g)<<3) & 0x7c0) | (((b)>>2) & 0x3e) | ((a) & 0x1)));
    }
};

extern Gfx rspinit_dl[];
extern Gfx rdpinit_dl[];
extern Gfx rdpinit_spr_dl[];

class TSprite {

    public:

    TSprite() = default;
    ~TSprite() = default;

    void load(Sprite const &);
    void preload(Sprite const &);

    u32 getWidth() const;
    u32 getHeight() const;

    TVec2S const & getPosition() const;
    void setPosition(TVec2S const &);

    TVec2F const & getScale() const;
    void setScale(TVec2F const &);

    TColor const & getColor() const;
    void setColor(TColor const &);

    u32 getAttributes() const;
    void setAttributes(u32);
    void setOnAttributes(u32);
    void setOffAttributes(u32);

    void draw();

    // initializes S2DEX. call before utilizing sprites
    static void init(TDynList2 *);

    // call after sprites are finished rendering.
    static void finalize();

    private:

    TVec2S mPosition { 0, 0 };
    TVec2F mScale { 1.0F, 1.0F };
    TColor mColor { 255, 255, 255, 255};
    u32 mAttributes { SP_FASTCOPY };
    Sprite mSpriteData;
    Gfx * mSpriteDL { nullptr };
    u32 mNumSpriteDL { 0 };

    static TDynList2 * sDynList;

};
