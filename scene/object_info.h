
#pragma once

#ifdef _cplusplus
extern "C"{
#endif

#include <nusys.h>
#include "math.h"
#include <stdbool.h> 

#define SIZE_TINY 0.1f
#define SIZE_JEWELRY 0.25f
#define SIZE_SMALL 0.5f
#define SIZE_MED 1.25f
#define SIZE_ANIMAL 3.5f
#define SIZE_LARGE 5.0f
#define SIZE_MAN 25.0f
#define SIZE_LAYER1 10.0f
#define SIZE_MASSIVE 50.0f
#define SIZE_GIANT 100.0f
#define SIZE_LAYER2 500.0f
#define SIZE_LAYER3 1000.0f
#define SIZE_IMPOSSIBLE 999999.0f

const float POWER_LEVELS[] = {
    SIZE_TINY,
    SIZE_JEWELRY,
    SIZE_SMALL,
    SIZE_MED,
    SIZE_ANIMAL,
    SIZE_LARGE,
    SIZE_LAYER1,
    SIZE_MAN,
    SIZE_MASSIVE,
    SIZE_GIANT,
    SIZE_LAYER2,
    SIZE_LAYER3,
    SIZE_IMPOSSIBLE
};

#define DRAWDIST_ALWAYS 8500.0f
#define DRAWDIST_THREEQUARTERS 6500.0f
#define DRAWDIST_HALF 4500.0f
#define DRAWDIST_QUARTER 2500.0f
#define DRAWDIST_EIGHTH 1500.0f
#define DRAWDIST_SIXTEENTH 1000.0f

struct TObjectData{
    Gfx * initializer;
    Gfx * mesh;
    char name[16];
    s16 colType;
    bool hasMeshCol;
    
    float sizex;
    float sizey;
    float sizez;

    float mass;
    s16 score;

    float drawDist;
};

extern const struct TObjectData gObjectDataList[];

#ifdef _cplusplus
}
#endif