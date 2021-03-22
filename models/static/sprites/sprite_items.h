#ifndef sp_items_h
#define sp_items_h

#include <PR/sp.h>

#define itemsTRUEIMAGEH	15
#define itemsTRUEIMAGEW	50
#define itemsIMAGEH	32
#define itemsIMAGEW	64
#define itemsBLOCKSIZEW	32
#define itemsBLOCKSIZEH	32
#define itemsSCALEX	1.0
#define itemsSCALEY	1.0
#define itemsMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap items_bitmaps[];
extern Gfx items_dl[];

#define NUM_items_BMS  (sizeof(items_bitmaps)/sizeof(Bitmap))

extern Sprite items_sprite;

#endif 

