#ifndef sp_time_h
#define sp_time_h

#include <PR/sp.h>

#define timeTRUEIMAGEH	19
#define timeTRUEIMAGEW	51
#define timeIMAGEH	32
#define timeIMAGEW	64
#define timeBLOCKSIZEW	32
#define timeBLOCKSIZEH	32
#define timeSCALEX	1.0
#define timeSCALEY	1.0
#define timeMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap time_bitmaps[];
extern Gfx time_dl[];

#define NUM_time_BMS  (sizeof(time_bitmaps)/sizeof(Bitmap))

extern Sprite time_sprite;

#endif 

