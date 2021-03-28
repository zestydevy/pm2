#ifndef sp_bg1_h
#define sp_bg1_h

#include <PR/sp.h>

#define bg1TRUEIMAGEH	240
#define bg1TRUEIMAGEW	320
#define bg1IMAGEH	256
#define bg1IMAGEW	320
#define bg1BLOCKSIZEW	32
#define bg1BLOCKSIZEH	32
#define bg1SCALEX	1.0
#define bg1SCALEY	1.0
#define bg1MODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap bg1_bitmaps[];
extern Gfx bg1_dl[];

#define NUM_bg1_BMS  (sizeof(bg1_bitmaps)/sizeof(Bitmap))

extern Sprite bg1_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|                                                |
|                                                |
|                                                |
|                rYrYrrYYrYYrYYr                 |
|                rYrYryYYrYYrYYr                 |
|               rrrrrrrrrrrrrrrr     r. rrr.     |
|       Yrrr.YYrYYYrYY.YYrrrYYYrYYYrYYYrYrYr     |
|      RYYYYRrYrYYyrYyYYrY  rYYrrrYrYYYrRYrr     |
|     .YyrRYYrYrYrYrYrrYYr  rrYYrYrrYYYrYYYr     |
|      rrrrrrrrrrrrrrrrrrr  rrrrrr rrrrrrrr      |
|                                                |
|         .r   YY   rr rYYYY.YrYYrY  YYYRr       |
|          r         R Y .Y  rr rrrr rr.rr       |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                                                |
|                 i @@   iI@@i@@                 |
|         i   .iii  iiIIiiIIIIiIi.IIIIiII        |
|                                                |
+------------------------------------------------+
#endif

