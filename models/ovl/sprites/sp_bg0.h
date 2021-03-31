#ifndef sp_bg0_h
#define sp_bg0_h

#include <PR/sp.h>

#define bg0TRUEIMAGEH	13
#define bg0TRUEIMAGEW	95
#define bg0IMAGEH	32
#define bg0IMAGEW	96
#define bg0BLOCKSIZEW	32
#define bg0BLOCKSIZEH	32
#define bg0SCALEX	1.0
#define bg0SCALEY	1.0
#define bg0MODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap bg0_bitmaps[];
extern Gfx bg0_dl[];

#define NUM_bg0_BMS  (sizeof(bg0_bitmaps)/sizeof(Bitmap))

extern Sprite bg0_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|    .                                           |
| I@ii@i@I@II@@@i@I@i@I@i  I@I@I@@IiI@I @II@i@@I |
|  IIII @II  @@I i@I i@I    I@.  @.i@i@ II@   @i |
|  i@  .@ @@ @@@i@@@i@@@.  I@@I  @i@II@ III@i @I |
+------------------------------------------------+
#endif

