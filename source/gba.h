/*
 * gba.h
 * this file contains definitions for the GBA hardware
 */

#ifndef GBA_H
#define GBA_H

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

/* control register for BG0 */
#define REG_BG0_CONTROL (*(volatile unsigned short*)0x4000008)
#define REG_BG1_CONTROL (*(volatile unsigned short*)0x400000a)

/* defines for laying out tile memory */
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1
#define CharBaseBlock(n) (((n) * 0x4000) + 0x6000000)
#define ScreenBaseBlock(n) (((n) * 0x800) + 0x6000000)

/* sets up 256 color mode for tile maps */
#define BG_COLOR256 0x80

/* identify palette memory */
#define BG_PALETTE_MEMORY ((unsigned short*)0x5000000)

/* different sizes for the tile maps */
#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512 0x8000
#define TEXTBG_SIZE_512x256 0x4000
#define TEXTBG_SIZE_512x512 0xC000


/* took these from the flappy bird game... */
#define BG_CHAR_BLOCK 0
#define BG_SCREEN_BLOCK0 16
#define BG_SCREEN_BLOCK1 18
#define BG_SCREEN_BLOCK2 20
#define BG_COLOR_PALETTE 0
#define BG_COLOR_256 1
#define BG_COLOR_16 0
#define COLOR_SHIFT 7
#define CHAR_SHIFT 2
#define PAL_SHIFT 7
#define SCREEN_SHIFT 8



/* scrolling registers for background 0 */
#define REG_BG0_XSCROLL *(volatile unsigned short*)0x4000010
#define REG_BG0_YSCROLL *(volatile unsigned short*)0x4000012
#define REG_BG1_XSCROLL *(volatile unsigned short*)0x4000014
#define REG_BG1_YSCROLL *(volatile unsigned short*)0x4000016

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

/* button identifiers */
#define BUTTON_RIGHT 16
#define BUTTON_LEFT  32
#define BUTTON_UP    64
#define BUTTON_DOWN  128
#define BUTTONS (*(volatile unsigned int*)0x04000130)

/* the vertical refresh register */
#define REG_DISPSTAT *(volatile unsigned short*)0x4000004

#endif


