/*
	ROM linkerscript file

	Main memory map

  	0x80000000  exception vectors, ...
  	0x80000400  zbuffer (size 320*240*2)
  	0x80025c00  codesegment
	0x80119e40
	      :  
  	0x8020F800  Game Heap
	0x8030F800  Audio Heap
  	0x8038F800  cfb 16b 3buffer (size 320*240*2*3)

   	Copyright (C) 1997-1999, NINTENDO Co,Ltd.	
*/

OUTPUT_ARCH (mips)

#include <nusys.h>

#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    __romPos += SIZEOF(.name);

#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ADDR(.name.noload) + SIZEOF(.name.noload); \
    _##name##SegmentBssSize = SIZEOF(.name.noload);

SECTIONS
{
   __romPos = 0;

   BEGIN_SEG(boot, 0x04000000)
   {
      build/obj/rom_header.o(.text);
      build/obj/boot.6102.o(.data);
   }
   END_SEG(boot)

   BEGIN_SEG(code, NU_SPEC_BOOT_ADDR)
   {
      /* text */
      build/obj/entry.o(.text);
      build/codesegment.o(.text*);
      /usr/lib/n64/PR/rspboot.o(.text);
      /usr/lib/n64/PR/n_aspMain.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.text);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.text);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.text);

      /* data */
      build/codesegment.o(.data*);
      /usr/lib/n64/PR/rspboot.o(.data*);
      /usr/lib/n64/PR/n_aspMain.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspL3DEX2.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DEX2.NoN.fifo.o(.data*);
      /usr/lib/n64/PR/gspF3DLX2.Rej.fifo.o(.data*);
      /usr/lib/n64/PR/gspS2DEX2.fifo.o(.data*);

      /* rodata */
      build/codesegment.o(.rodata*);
   }
   END_SEG(code)
   BEGIN_NOLOAD(code)
   {
      build/codesegment.o(COMMON);
      build/codesegment.o(.scommon*);
      build/codesegment.o(.bss*);
      build/obj/sp_logo1.o(.bss*);
      build/obj/sp_logo2.o(.bss*);
      build/obj/sp_bg1.o(.bss*);
      build/obj/sp_bg2.o(.bss*);
      build/obj/sp_bg3.o(.bss*);
      build/obj/sp_bg4.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(code)

   BEGIN_SEG(logo_ovl, _codeSegmentBssEnd)
   {
      build/obj/sp_logo1.o(.data*);
      . = ALIGN(2);
      build/obj/sp_logo2.o(.data*);
      . = ALIGN(2);
   }
   END_SEG(logo_ovl)
   BEGIN_NOLOAD(logo_ovl)
   {
      
   }
   END_NOLOAD(logo_ovl)

   BEGIN_SEG(title_ovl, _codeSegmentBssEnd)
   {
      build/obj/sp_bg1.o(.data*);
      build/obj/sp_bg2.o(.data*);
      build/obj/sp_bg3.o(.data*);
      build/obj/sp_bg4.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(title_ovl)
   BEGIN_NOLOAD(title_ovl)
   {
      
   }
   END_NOLOAD(title_ovl)
   
   BEGIN_SEG(midibank, __romPos)
   {
      sound/n64st1.wbk.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(midibank)

   BEGIN_SEG(miditable, __romPos)
   {
      sound/n64st1.ptr.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(miditable)

   BEGIN_SEG(seq, __romPos)
   {
      . = ALIGN(0x10);
   }
   END_SEG(seq)

   BEGIN_SEG(sfx, __romPos)
   {
      sound/N64ST1.bfx.o(.data*);
      . = ALIGN(0x10);
   }
   END_SEG(sfx)

   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
