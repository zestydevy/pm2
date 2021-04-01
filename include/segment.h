/*
   segment.h

   Declare external reference of the segment symbol that makerom outputs 

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/


#ifndef SEGMENT_H
#define SEGMENT_H

extern char _codeSegmentStart[];         /* code segment start */
extern char _codeSegmentBssEnd[];           /* code segment end */
extern char _countdown_ovlSegmentStart[];
extern char _ranka_ovlSegmentStart[];
extern char _world_ovlSegmentStart[];
extern char _bird_ovlSegmentStart[];
extern char _eagle_ovlSegmentStart[];
extern char _menu_ovlSegmentStart[];

extern u8 _logo_ovlSegmentRomStart[];
extern u8 _logo_ovlSegmentRomEnd[];
extern u8 _title_ovlSegmentRomStart[];
extern u8 _title_ovlSegmentRomEnd[];
extern u8 _crash_ovlSegmentRomStart[];
extern u8 _crash_ovlSegmentRomEnd[];
extern u8 _world_ovlSegmentRomStart[];
extern u8 _world_ovlSegmentRomEnd[];
extern u8 _bird_ovlSegmentRomStart[];
extern u8 _bird_ovlSegmentRomEnd[];
extern u8 _eagle_ovlSegmentRomStart[];
extern u8 _eagle_ovlSegmentRomEnd[];
extern u8 _menu_ovlSegmentRomStart[];
extern u8 _menu_ovlSegmentRomEnd[];
extern u8 _countdown_ovlSegmentRomStart[];
extern u8 _countdown_ovlSegmentRomEnd[];
extern u8 _result_ovlSegmentRomStart[];
extern u8 _result_ovlSegmentRomEnd[];
extern u8 _ranka_ovlSegmentRomStart[];
extern u8 _ranka_ovlSegmentRomEnd[];
extern u8 _rankb_ovlSegmentRomStart[];
extern u8 _rankb_ovlSegmentRomEnd[];
extern u8 _rankc_ovlSegmentRomStart[];
extern u8 _rankc_ovlSegmentRomEnd[];
extern u8 _rankd_ovlSegmentRomStart[];
extern u8 _rankd_ovlSegmentRomEnd[];
extern u8 _rankf_ovlSegmentRomStart[];
extern u8 _rankf_ovlSegmentRomEnd[];
extern u8 _seqSegmentRomStart[];
extern u8 _seqSegmentRomEnd[];
extern u8 _midibankSegmentRomStart[];
extern u8 _midibankSegmentRomEnd[];
extern u8 _miditableSegmentRomStart[];
extern u8 _miditableSegmentRomEnd[];
extern u8 _sfxSegmentRomStart[];
extern u8 _sfxSegmentRomEnd[];

extern u8 _testRelSegmentRomStart[];
extern u8 _testRelSegmentRomEnd[];
extern u8 _testRelSegmentStart[];
extern u8 _testRelSegmentEnd[];
extern u8 _testSegmentReloc[];

#endif /* SEGMENT_H */
