//
// ci.c
//

#include <ultra64.h>
#include <nusys.h>
#include <string.h>
#include "ci.h"



unsigned char	buffer[512] = {0, };

// put the debug area at the 63MB area in SDRAM
u32 ci_dbg_area = 0x3F00000;
u32 ci_dbg_area_size = 1*1024*1024;

// base address for all registers
u32 ci_base = CART_BASE_UNCACHED + CI_REG_BASE;

//
// ci_wait
//
// Waits until CI operation is idle or complete.
// Arguments:
//
// Return value:
//  -1 : timed out (several seconds)
//   0 : idle
//
u32 ci_wait()
{
	long timeout = 0;
	u32 ret;
	do{
		osPiReadIo(ci_base + CI_REG_STATUS, &ret); 
		timeout++;
		if(timeout == 4000000){
			return -1;
		}
	}while( (ret >> 8) & 0x10 );
	return 0;
}


void ci_usb_printf_line(char *str)
{
	int i;
	int len_old = strlen(str) + 1;
	int len_new = len_old;

	// truncate to fit buffer
	if(len_old > 512){
		len_old = 512;
		len_new = 512;
	}
	memcpy(buffer, str, len_old);
	// pad out length to 32bit alignment
	if(len_old % 4 != 0) {
		len_new = (len_old/4)*4 + 4;
		for(i = len_old; i < len_new; i++) buffer[i] = 0;
	}
	ci_usb_write(buffer, len_new, 0x01); // 01 = printf channel
}

void ci_usb_printf(char *str)
{
	ci_usb_printf_line(str);
	ci_usb_printf_line("\n");
}

u32 ci_usb_status_arm()
{
	u32 a;
	osPiReadIo(ci_base + CI_REG_USB_CMDSTAT, &a); 
	return a & 0xf;
}

u32 ci_usb_status_write()
{
	u32 a;
	osPiReadIo(ci_base + CI_REG_USB_CMDSTAT, &a); 
	return (a >> 4) & 0xf;
}

//
// ci_usb_spin_write
//
// Spins until write buffer is free.
// This can imply either there was no data to send, or it has finished sending.
// Arguments:
//
// Return value:
//
void ci_usb_spin_write()
{
	while(ci_usb_status_write() != CI_STAT_USB_WR_IDLE);
}


//
// ci_usb_write
//
// Writes a buffer of specified length to the debug pool and sends to hardware.
// Does not check for completion.
// Arguments:
//   buf : pointer to data
//   length : length in bytes
//   type : block type
// Return value:
//  -1 : already armed or busy
//   0 : success, armed
//
u32 ci_usb_write(u8 *buf, u32 length, u8 type)
{
	OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
	
	//u32 status;
	//status = ci_usb_status_write();
	//if(status != CI_STAT_USB_WR_IDLE) return -1;
	ci_usb_spin_write();
		
	ci_rom_writable(1);
	osWritebackDCacheAll();
	osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_WRITE, 
		CART_BASE_UNCACHED + ci_dbg_area, buf, length, &dmaMesgQ);
	(void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
	
	osPiWriteIo(ci_base + CI_REG_USB_PARAM0, (ci_dbg_area) >> 1);
	osPiWriteIo(ci_base + CI_REG_USB_PARAM1, (length & 0xffffff) | (type << 24));
	osPiWriteIo(ci_base + CI_REG_USB_CMDSTAT, CI_CMD_USB_WR); 
	return 0;
}

//
// ci_usb_arm
//
// Arms the buffer to receive data from host up to its maximum size.
// Arguments:
//   
// Return value:
//  -2 : write in progress, finish that first
//  -1 : already armed or busy
//   0 : success, armed
//
u32 ci_usb_arm()
{
	u32 status;
	status = ci_usb_status_arm();
	// exit with error if it is already armed
	if( status == CI_STAT_USB_ARM_ARMING ||
		status == CI_STAT_USB_ARM_ARMED) return -1;
	// make sure a write wasn't in progress
	status = ci_usb_status_write();
	if(status == CI_STAT_USB_WR_BUSY) return -2;
	// send parameters and command
	osPiWriteIo(ci_base + CI_REG_USB_PARAM0, (ci_dbg_area) >> 1);
	osPiWriteIo(ci_base + CI_REG_USB_PARAM1, (ci_dbg_area_size & 0xffffff));
	osPiWriteIo(ci_base + CI_REG_USB_CMDSTAT, CI_CMD_USB_ARM); 
	return 0;
}

//
// ci_usb_disarm
//
// Disarms a currently armed buffer.
// Arguments:
//   
// Return value:
//  -1 : fail, weren't armed so nothing was done
//   0 : success, arming was canceled
//
u32 ci_usb_disarm()
{
	u32 status;
	status = ci_usb_status_arm();
	if(status == CI_STAT_USB_ARM_UNARMED_IDLE) return -1;
	// send DISARM  (ARM CANCEL) cui command
	osPiWriteIo(ci_base + CI_REG_USB_CMDSTAT, CI_CMD_USB_DISARM); 
	while(1){
		status = ci_usb_status_arm();
		if( status == CI_STAT_USB_ARM_UNARMED_IDLE) return 0;
	}
}

//
// ci_usb_spin_recv
//
// Spins until an armed buffer receives data.
// Arguments:
//   
// Return value:
//  -1 : fail, weren't armed
//   0 : success, data waits in buffer
//
u32 ci_usb_spin_recv()
{
	// note: this is a polling implementation that waits until the armed buffer 
	// receives data from host.
	// compatible with both HW1/HW2, but HW2 supports cart interrupt which would 
	// be much more efficient. a multithreaded application would yield the thread here to 
	// resume other duties and periodically check back in.
	u32 status;
	while(1){
		status = ci_usb_status_arm();
		// exit with error if we were never armed
		if(status == CI_STAT_USB_ARM_UNARMED_IDLE) return -1;
		// wait til we are unarmed either with data in buffer or not
		if( status == CI_STAT_USB_ARM_UNARMED_DATA) break;
	}
	return 0;
}


//
// ci_usb_recv_service
//
// Services an armed receive buffer.
// Copies out the data from the cartram debug pool into the specified buffer.
// If the block's overall length is too big to fit into the armed debug pool, 
// you should call this multiple times and copy the data repeatedly.
// Arguments:
//   buf : ptr to where you want the data to go.
//   length : bytes received and copied
//   length_remain : bytes that are yet to be serviced'
//   type : block type
// Return value:
//  -1 : fail, disarmed or no data yet
//   0 : success, no more chunks remain in block to be serviced
//   1 : success, more chunks must be serviced as well
//
u32 ci_usb_recv_service(u8 *buf, u32 *length, u32 *length_remain, u8 *type)
{
	OSIoMesg	dmaIoMesgBuf;
    OSMesgQueue dmaMesgQ;
    OSMesg	dmaMesgBuf;
	
	u32 status = ci_usb_status_arm();
	u32 retval;
	
	// exit with error if we were never armed
	if( status == CI_STAT_USB_ARM_UNARMED_IDLE ||
		status == CI_STAT_USB_ARM_ARMED) return -1;

	// read back the reported info about our block progress
	osPiReadIo(ci_base + CI_REG_USB_PARAM0, &retval); 
	*length = retval & 0xFFFFFF;
	*type = (retval >> 24) & 0xff;
	osPiReadIo(ci_base + CI_REG_USB_PARAM1, &retval); 
	*length_remain = retval & 0xFFFFFF;
	
	osWritebackDCacheAll();
	osCreateMesgQueue(&dmaMesgQ, &dmaMesgBuf, 1);
	osPiStartDma(&dmaIoMesgBuf, OS_MESG_PRI_NORMAL, OS_READ, 
		CART_BASE_UNCACHED + ci_dbg_area, buf, *length, &dmaMesgQ);
	(void)osRecvMesg(&dmaMesgQ, NULL, OS_MESG_BLOCK);
	
	if(*length_remain > 0) return 1; else return 0;
}

//
// ci_rom_writable
//
// Sets cartram as writable or read-only (default)
// Arguments:
//   enable : allows data to be DMA/written to cartram from N64
// Return value:
//
void ci_rom_writable(u32 enable)
{
	ci_wait();
	osPiWriteIo(ci_base + CI_REG_COMMAND, enable ? CI_CMD_ENABLE_ROMWR : CI_CMD_DISABLE_ROMWR); 
	ci_wait();
}


