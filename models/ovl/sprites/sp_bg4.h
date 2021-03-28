#ifndef sp_bg4_h
#define sp_bg4_h

#include <PR/sp.h>

#define bg4TRUEIMAGEH	123
#define bg4TRUEIMAGEW	320
#define bg4IMAGEH	128
#define bg4IMAGEW	320
#define bg4BLOCKSIZEW	32
#define bg4BLOCKSIZEH	32
#define bg4SCALEX	1.0
#define bg4SCALEY	1.0
#define bg4MODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap bg4_bitmaps[];
extern Gfx bg4_dl[];

#define NUM_bg4_BMS  (sizeof(bg4_bitmaps)/sizeof(Bitmap))

extern Sprite bg4_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|                                                |
|CCCCCCCG GG@@ GGGGG GG  @CCcCgggg@ YYYYYYYYCCCCC|
|CCCCCCggggYYYYYYYGGgGGGCCCCCCCCCYYYYYYYyyYYYYCCC|
|CCCCCggggggggIGYgIggggGYYYbbbGGYYYYYyyYYYYYYYYCC|
|CCCCCCCCCIYYYYYYYGcGGGCGYYcccccbbgYYYYYYYYYYYYYC|
|CCCCCCCCCCCCCgggggGYggCccccccccYygYYYYYYYYYYYYYY|
|CCCCCyryCCCCCCCgggrigggcgccCYYYyYyyYYYYYYYYYYYCC|
|CCcccyyyycccccCCCCCCCCggggggggYyYYyYyyYYYGggCCC@|
|CCcccccccccGcCGGCCCCCCCggggggyYyyyYYYYYgCCCCCCCC|
|CCccccccYYYYGCGCCCCCCCCCCCCCggGGYYgggCCCCCCCCCCB|
|CCCCCCCGGCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBb|
|CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBBbbbb|
+------------------------------------------------+
#endif

