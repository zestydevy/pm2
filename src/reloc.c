//Include Library
#include <nusys.h>

//Function to load the overlay from ROM
#define DMA_FUNC nuPiReadRom

void bzero(void *, int);

//Apply relocations to the segment
static void ExecRelocation(void *virt_ram_addr, void *ram_addr, u32 *ram_reloc)
{
	u32 i;
	u32 pair_val[32];
	u32 *pair_addr[32];
	void *section_ptr[4];
	u32 offset = (u32)ram_addr-(u32)virt_ram_addr;
	//Setup section pointers
	section_ptr[0] = NULL;
	section_ptr[1] = ram_addr;
	section_ptr[2] = (u8 *)ram_addr+ram_reloc[0];
	section_ptr[3] = ((u8 *)section_ptr[2])+ram_reloc[1];
	for(i=0; i<ram_reloc[4]; i++) {
		//Read relocation data
		u32 word = ram_reloc[i+5];
		u32 *target = (u32 *)(((u8 *)section_ptr[word >> 30])+(word & 0xFFFFFF));
		u32 type = (word >> 24) & 0x3F;
		if(word >> 30 == 0) {
			return;
		}
		switch(type) {
			case 2:
			//R_MIPS_32
			//Used for pointers in data sections
				*target += offset;
				break;
				
			case 4:
			//R_MIPS_26
			//Used for JALs
				{
					 u32 new_target, old_target;
					 old_target = (*target & 0x3FFFFFF) << 2;
					 new_target = (old_target + offset) & 0xFFFFFFF;
					 *target &= ~0x3FFFFFF;
					 *target |= (new_target/4);
				}
				break;
				
			case 5:
			//R_MIPS_HI16
			//Start LUI/ADDIU pair
				{
					u32 reg = (*target >> 16) & 0x1F;
					pair_addr[reg] = target;
					pair_val[reg] = (*target & 0xFFFF) << 16;
				}
				break;
				
			case 6:
			//R_MIPS_LO16
			//Finish LUI/ADDIU pair
				{
					u32 reg = (*target >> 21) & 0x1F;
					u32 addr, hi, lo;
					s16 val;
					val = (s16)(*target & 0xFFFF);
					pair_val[reg] += val;
					addr = pair_val[reg] + offset;
					lo = addr & 0xFFFF;
					hi = (addr >> 16) + ((lo & 0x8000) >> 15);
					*pair_addr[reg] = (*pair_addr[reg] & 0xFFFF0000) | hi;
					*target = (*target & 0xFFFF0000) | lo;
				}
				break;
				
			default:
				return;
		}
	}
}

void RelocSegLoad(u32 rom_start, u32 rom_end, void *virt_ram_start, void *virt_ram_end, void *reloc, void *ram_addr)
{
	u32 rom_size = rom_end-rom_start;
	u32 ram_size = (u32)virt_ram_end-(u32)virt_ram_start;
	void *bss_ptr = (u8 *)ram_addr+rom_size;
	u32 *ram_reloc = (u32 *)(((u32)reloc-(u32)virt_ram_start)+(u8 *)ram_addr);
	//Load overlay to RAM and relocate
	DMA_FUNC(rom_start, ram_addr, rom_size);
	ExecRelocation(virt_ram_start, ram_addr, ram_reloc);
	//Finish loading overlay to RAM
	bzero(bss_ptr, ram_reloc[3]);
	osWritebackDCache(ram_addr, ram_size);
    osInvalICache(ram_addr, ram_size);
}