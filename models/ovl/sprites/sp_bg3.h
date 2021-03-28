#ifndef sp_bg3_h
#define sp_bg3_h

#include <PR/sp.h>

#define bg3TRUEIMAGEH	165
#define bg3TRUEIMAGEW	320
#define bg3IMAGEH	192
#define bg3IMAGEW	320
#define bg3BLOCKSIZEW	32
#define bg3BLOCKSIZEH	32
#define bg3SCALEX	1.0
#define bg3SCALEY	1.0
#define bg3MODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap bg3_bitmaps[];
extern Gfx bg3_dl[];

#define NUM_bg3_BMS  (sizeof(bg3_bitmaps)/sizeof(Bitmap))

extern Sprite bg3_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|@@@@@@@@@@@@@@@@@@@@@IIIIII@@I@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|
|iiIII@@@@@@@@@@@@@@@@@@@@@IIiIII@@@@@@@Iii.iIIIi|
|    .iIii@@@@@@IIIII@@IIIII .ii.iiIiiii      .  |
|         IIIIiiii  iiiii.ii                     |
+------------------------------------------------+
#endif

