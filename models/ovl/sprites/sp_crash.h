#ifndef sp_crash_h
#define sp_crash_h

#include <PR/sp.h>

#define crashTRUEIMAGEH	240
#define crashTRUEIMAGEW	320
#define crashIMAGEH	256
#define crashIMAGEW	320
#define crashBLOCKSIZEW	32
#define crashBLOCKSIZEH	32
#define crashSCALEX	1.0
#define crashSCALEY	1.0
#define crashMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap crash_bitmaps[];
extern Gfx crash_dl[];

#define NUM_crash_BMS  (sizeof(crash_bitmaps)/sizeof(Bitmap))

extern Sprite crash_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|                                                |
|                                                |
|                                                |
|      @  @ @      @      @ @@   @@              |
|              @@   @    @   @   @@   @@   @     |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|          @    @@                               |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
+------------------------------------------------+
#endif

