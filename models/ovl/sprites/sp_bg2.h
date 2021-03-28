#ifndef sp_bg2_h
#define sp_bg2_h

#include <PR/sp.h>

#define bg2TRUEIMAGEH	187
#define bg2TRUEIMAGEW	272
#define bg2IMAGEH	192
#define bg2IMAGEW	288
#define bg2BLOCKSIZEW	32
#define bg2BLOCKSIZEH	32
#define bg2SCALEX	1.0
#define bg2SCALEY	1.0
#define bg2MODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap bg2_bitmaps[];
extern Gfx bg2_dl[];

#define NUM_bg2_BMS  (sizeof(bg2_bitmaps)/sizeof(Bitmap))

extern Sprite bg2_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|                                                |
|                                   iiiii iiii   |
|                             .iIIIIIIIIiiI@IIIi |
|                            iiI@@@I@@@III@@@IIIi|
|             .      .ii.iiiII@@@@@@@@@@@@@@@@IIi|
|           iIIIii iiiIIiII@@@@@@@@@@@@@@@@@@@@I |
|    . iIiI@@@@@@@IIIIII@@@@@@@@@@@@@@@@@@@@@@@I |
|   .IIII@@@@@@@@@@@@@I@@@@@@@@@@@@@@@@@@@@@@@@I |
|   I@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Iii |
|  I@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@IIi.i |
| iI@@@@@@@@@@@@@@@@@@@@@IIiI@IIiiIiiIiiIIIiii   |
|  I@@@@@@@@@@@@@@@@@@@@@Ii   .        iii       |
|  iI@@@@@@@@@@@@@@@@@IIIii                      |
|   iII@@@@@@@@@@@IIIiiii.                       |
|   iiIIIII@@@@@Iiiiii.                          |
|   ii  .iIIIIII.                                |
|                        iiiIiiiiii.             |
|                      iII@@IIIIIIIIIii..        |
|                    iiI@@@@@@@@@@@@@@@@Iii      |
|                  iiI@@@@@@@@@@@@@@@@@@@Ii      |
|                 .iII@@@@@@@@@@@@@@@@@@@@Ii     |
|                  iiIII@@@@@@@@@@@@@@@@@@I      |
+------------------------------------------------+
#endif

