#ifndef sp_black_h
#define sp_black_h

#include <PR/sp.h>

#define blackTRUEIMAGEH	32
#define blackTRUEIMAGEW	32
#define blackIMAGEH	32
#define blackIMAGEW	32
#define blackBLOCKSIZEW	32
#define blackBLOCKSIZEH	32
#define blackSCALEX	320
#define blackSCALEY	240
#define blackMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap black_bitmaps[];
extern Gfx black_dl[];

#define NUM_black_BMS  (sizeof(black_bitmaps)/sizeof(Bitmap))

extern Sprite black_sprite;

#endif 

