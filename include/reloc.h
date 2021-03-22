#ifndef RELOC_H
#define RELOC_H

#ifdef __cplusplus
extern "C" {
#endif

//Library includes
#include <ultra64.h>

//Defines

//Get corresponding real RAM address for a virtual RAM address
//addr is the address you want to translate
//virt_ram_base is the starting virtual RAM address of the segment
//ram_base is the load address of the Segment
#define RELOC_GET_RAM_ADDR(addr, virt_ram_base, ram_base) (void *)((addr ? ((u8 *)addr+((u32)ram_base-(u32)virt_ram_base)) : NULL))

//Declarations

//Loads a relocatable overlay to RAM
//rom_start and rom_end are the start and end ROM address of the segment
//virt_ram_start and virt_ram_end are the start and end virtual RAM Address of the segment
//reloc is the symbol _%sSegmentReloc with %s being the segment name specified in the spec File
//ram_addr is the load address of the segment
void RelocSegLoad(u32 rom_start, u32 rom_end, void *virt_ram_start, void *virt_ram_end, void *reloc, void *ram_addr);

#ifdef __cplusplus
}
#endif

#endif