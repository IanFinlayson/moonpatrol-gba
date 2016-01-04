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
#define REG_BG0CNT (*(volatile unsigned short*)0x4000008)
#define REG_BG1CNT (*(volatile unsigned short*)0x400000a)
#define REG_BG2CNT (*(volatile unsigned short*)0x400000c)
#define REG_BG3CNT (*(volatile unsigned short*)0x400000e)

/* defines for laying out tile memory */
#define SCREEN_SHIFT 8
#define CharBaseBlock(n) (((n) * 0x4000) + 0x6000000)
#define ScreenBaseBlock(n) (((n) * 0x800) + 0x6000000)

/* sets up 256 color mode for tile maps */
#define BG_COLOR256 0x80

/* identify palette memory */
#define BG_PALETTE_MEMORY ((unsigned short*)0x5000000)
#define OBJ_PALETTE_MEMORY ((unsigned short*) 0x5000200)

/* scrolling registers for backgrounds */
#define REG_BG0HOFS *(volatile unsigned short*)0x4000010
#define REG_BG0VOFS *(volatile unsigned short*)0x4000012
#define REG_BG1HOFS *(volatile unsigned short*)0x4000014
#define REG_BG1VOFS *(volatile unsigned short*)0x4000016
#define REG_BG2HOFS *(volatile unsigned short*)0x4000018
#define REG_BG2VOFS *(volatile unsigned short*)0x400001a
#define REG_BG3HOFS *(volatile unsigned short*)0x400001c
#define REG_BG3VOFS *(volatile unsigned short*)0x400001e

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


