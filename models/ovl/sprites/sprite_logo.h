#ifndef sp_logo_h
#define sp_logo_h

#include <PR/sp.h>

#define logoTRUEIMAGEH	183
#define logoTRUEIMAGEW	256
#define logoIMAGEH	192
#define logoIMAGEW	256
#define logoBLOCKSIZEW	32
#define logoBLOCKSIZEH	32
#define logoSCALEX	1.0
#define logoSCALEY	1.0
#define logoMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap logo_bitmaps[];
extern Gfx logo_dl[];

#define NUM_logo_BMS  (sizeof(logo_bitmaps)/sizeof(Bitmap))

extern Sprite logo_sprite;

#endif 

