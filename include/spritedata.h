#ifndef  __SPRITEDATA_H
#define  __SPRITEDATA_H

#include <ultra64.h>
#include <PR/gs2dex.h>

#include "spritedata.h"

typedef struct spriteExEntry
{
    uObjTxtr * textureData;
    uObjMtx * spriteMtx;
    uObjSprite * spriteData;
} TSpriteExEntry;

#endif