#ifndef sp_white_h
#define sp_white_h

#include <PR/sp.h>

#define whiteTRUEIMAGEH	32
#define whiteTRUEIMAGEW	32
#define whiteIMAGEH	32
#define whiteIMAGEW	32
#define whiteBLOCKSIZEW	32
#define whiteBLOCKSIZEH	32
#define whiteSCALEX	320
#define whiteSCALEY	240
#define whiteMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap white_bitmaps[];
extern Gfx white_dl[];

#define NUM_white_BMS  (sizeof(white_bitmaps)/sizeof(Bitmap))

extern Sprite white_sprite;

#endif 

