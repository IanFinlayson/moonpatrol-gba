/*
 * gba.h
 * this file contains definitions for the GBA hardware registers and constants
 */

#ifndef GBA_H
#define GBA_H

/* the mode flags */
#define MODE_0 0x0
#define MODE_1 0x1
#define MODE_2 0x2
#define MODE_3 0x3
#define MODE_4 0x4
#define MODE_5 0x5

/* the flags to enable various background layers */
#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800

/* flags to set object handling */
#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40
#define OBJ_ENABLE 0x1000

/* the graphics display control register */
#define REG_DISPCNT ((volatile unsigned long*) 0x4000000)

/* screen dimenstions */
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

/* the screen memory pointer */
#define SCREEN ((volatile unsigned short*) 0x6000000)

/* control registers for background controls */
#define REG_BG0CNT (*(volatile unsigned short*) 0x4000008)
#define REG_BG1CNT (*(volatile unsigned short*) 0x400000a)
#define REG_BG2CNT (*(volatile unsigned short*) 0x400000c)
#define REG_BG3CNT (*(volatile unsigned short*) 0x400000e)

/* defines for laying out tile memory */
#define CharBaseBlock(n) (((n) * 0x4000) + 0x6000000)
#define ScreenBaseBlock(n) (((n) * 0x800) + 0x6000000)

/* identify palette memorys */
#define BG_PALETTE_MEMORY ((volatile unsigned short*) 0x5000000)
#define OBJECT_PALETTE_MEMORY ((volatile unsigned short*) 0x5000200)

/* memory address for sprite image data */
#define OBJECT_DATA_MEMORY ((volatile unsigned short*) 0x6010000)

/* memory addess for object attributes OAM */
#define OBJECT_ATTRIBUTE_MEMOORY ((volatile unsigned short*) 0x7000000)

/* scrolling registers for backgrounds */
#define REG_BG0HOFS (*(volatile unsigned short*) 0x4000010)
#define REG_BG0VOFS (*(volatile unsigned short*) 0x4000012)
#define REG_BG1HOFS (*(volatile unsigned short*) 0x4000014)
#define REG_BG1VOFS (*(volatile unsigned short*) 0x4000016)
#define REG_BG2HOFS (*(volatile unsigned short*) 0x4000018)
#define REG_BG2VOFS (*(volatile unsigned short*) 0x400001a)
#define REG_BG3HOFS (*(volatile unsigned short*) 0x400001c)
#define REG_BG3VOFS (*(volatile unsigned short*) 0x400001e)

/* the registers which trigeer DMA operations to start */
#define REG_DMA_SOURCE *(volatile unsigned int*) 0x40000D4
#define REG_DMA_DESTINATION *(volatile unsigned int*) 0x40000D8
#define REG_DMA_COUNT *(volatile unsigned int*) 0x40000DC

/* flags to enable direct memory access */
#define DMA_ENABLE 0x80000000
#define DMA_TIMING_IMMEDIATE 0x00000000

/* transfer 16 or 32 bit memory? */
#define DMA_16 0x00000000
#define DMA_32 0x04000000

/* combined flags which turn on DMA right away for 16 or 32 bits */
#define DMA_32_NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32)
#define DMA_16_NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_16)

/* button identifiers */
#define BUTTON_A 0x001
#define BUTTON_B 0x002
#define BUTTON_SELECT 0x004
#define BUTTON_START 0x008
#define BUTTON_RIGHT 0x010
#define BUTTON_LEFT 0x020
#define BUTTON_UP 0x040
#define BUTTON_DOWN 0x080
#define BUTTON_R 0x100
#define BUTTON_L 0x200
#define BUTTONS (*(volatile unsigned int*) 0x04000130)

/* the vertical refresh register */
#define REG_DISPSTAT (*(volatile unsigned short*) 0x4000004)

#endif


