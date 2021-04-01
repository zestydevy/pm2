#ifndef sp_credits_h
#define sp_credits_h

#include <PR/sp.h>

#define creditsTRUEIMAGEH	240
#define creditsTRUEIMAGEW	320
#define creditsIMAGEH	256
#define creditsIMAGEW	320
#define creditsBLOCKSIZEW	32
#define creditsBLOCKSIZEH	32
#define creditsSCALEX	1.0
#define creditsSCALEY	1.0
#define creditsMODE	SP_Z | SP_OVERLAP | SP_TRANSPARENT

// extern varaibles 
extern Bitmap credits_bitmaps[];
extern Gfx credits_dl[];

#define NUM_credits_BMS  (sizeof(credits_bitmaps)/sizeof(Bitmap))

extern Sprite credits_sprite;

#endif 

#if 0	/* Image preview */
+------------------------------------------------+
|                                                |
|        bbb  i rrI  IrC .ir  rYr rY   rr        |
|        II. rr irMM  Y. .Gg rYBB mYr Byyi       |
|                                                |
|     bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb    |
|     bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb    |
|     bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbBbbbbbbb    |
|     bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb    |
|     bbbbbbbbbbbbbbbbbibbbibbbbbbbbbbbbbbbbb    |
|     bbbbIIbbbbbbbbbBBBBBBbBBbbbbbbbbbbbbbbb    |
|     bbbbbbbbbbbbbbBBiBIbIbiBBbbbbBbbbIbbbbb    |
|     bbbbbbibbbbbbbibiiCiBiibibbBbbbbYbbb.bb    |
|     b..bBCBBbbbbbiiiIBBCBBIiiibbbbbCCCYb...    |
|     .b.bCcCBbbbbbbbbbBBbBbbb.bbbbbbCCICb...    |
|     .bbBBcCBbbb.bbBBbBccBBbBIbb...bBCiBBbb.    |
|     bbbCccCbbb.bbbBBbbbbcbbBBbbbb.bBCcCCbb.    |
|     bCCCiCBCBCCCCBBbIYYYYYbBBBCBCCBCBCcCCBC    |
|     CBBCicBBbbBbbBBBBBBCBBBBBBBbbCBCCcbCBB@    |
|     bCbbbbIbbBBMBBMMMBMMMMMMBBBBMbbbCbbbbCb    |
|     bbbbbBCMMmmBBMMMMMMMMMMMMMMMMMMMbbbbCBb    |
|                                                |
|            @      @      @          @ @        |
|        @@@   @  @      @@ @@       @  @  @@    |
|                                                |
+------------------------------------------------+
#endif

