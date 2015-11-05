/*
 * gba.h
 * this file contains definitions for the GBA hardware
 */

#ifndef GBA_H
#define GBA_H

/*
 * graphics modes and basic registers and things
 */

/* the mode 0 flag */
#define MODE_0 0x0

/* the flag to enable various background layers */
#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800

/* the graphics display control register */
#define REG_DISPCNT ((volatile unsigned long*) 0x4000000)

/* the screen memory pointer */
#define SCREEN ((volatile unsigned short*) 0x6000000)

/* the location of background 0 */
#define REG_BG0_PTR *(volatile unsigned short*)0x4000008

/* TODO what's this about */
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1
#define CharBaseBlock(n) (((n)*0x4000)+0x6000000)                                  
#define ScreenBaseBlock(n) (((n)*0x800)+0x6000000)                                 


/* sets up 256 color mode for tile maps */
#define BG_COLOR256 0x80

/* identify palette memory */
#define BG_PALETTE_MEMORY ((unsigned short*)0x5000000)

/* different sizes for the tile maps */
#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512 0x8000
#define TEXTBG_SIZE_512x256 0x4000
#define TEXTBG_SIZE_512x512 0xC000


/* scrolling registers for background 0 */
#define REG_BG0_XSCROLL *(volatile unsigned short*)0x4000010
#define REG_BG0_YSCROLL *(volatile unsigned short*)0x4000012



/*
 * DMA copy stuff
 */

/* function to perform fast DMA memory copies */
void dma_memcpy(void* source, void* dest, unsigned count, unsigned mode);

/* the registers which trigeer a DMA operation to start */
#define REG_DMA_SOURCE *(volatile unsigned int*)0x40000D4
#define REG_DMA_DESTINATION *(volatile unsigned int*)0x40000D8
#define REG_DMA_COUNT *(volatile unsigned int*)0x40000DC

/* flag to enable direct memory access */
#define DMA_ENABLE 0x80000000

/* start it right away */
#define DMA_TIMING_IMMEDIATE 0x00000000

/* transfer 16 or 32 bit memory? */
#define DMA_16 0x00000000
#define DMA_32 0x04000000

/* combined flags which turn on DMA right away for 16 or 32 bits */
#define DMA_32_NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32)
#define DMA_16_NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_16)

#endif


