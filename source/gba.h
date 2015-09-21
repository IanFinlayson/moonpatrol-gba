/*
 * gba.h
 * this file contains definitions for the GBA hardware
 */

#ifndef GBA_H
#define GBA_H

/* graphics mode stuff */
#define MODE_3 0x03
#define BACKGROUND2 0x400
#define REG_DISPCNT ((volatile unsigned long*) 0x4000000)

/* graphics stuff */
#define SCREEN ((volatile unsigned short*) 0x6000000)

#endif

