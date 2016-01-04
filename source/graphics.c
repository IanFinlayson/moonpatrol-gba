/*
 * graphics.c
 * this file contains graphics routines
 */

#include "gba.h"

/* function to use the GBA's hardware memory copy */
void dma_memcpy(void* source, void* dest, unsigned count, unsigned mode) {
    /* ensure that one of the valid modes are passed */
    if (mode != DMA_16_NOW && mode != DMA_32_NOW) {
        return;
    }

    /* trigger the DMA hardware copy to run - the CPU is
     * automatically suspended until the transfer is done */
    REG_DMA_SOURCE = (unsigned int) source;
    REG_DMA_DESTINATION = (unsigned int) dest;
    REG_DMA_COUNT = count | mode;
}


/* initialize a background */
void init_background(int bg, int priority, int screenblock) {
    /* get the appropriate control register */
    unsigned short* control;
    switch (bg) {
        case 0: control = (volatile unsigned short*) 0x4000008; break;
        case 1: control = (volatile unsigned short*) 0x400000a; break;
        case 2: control = (volatile unsigned short*) 0x400000c; break;
        case 3: control = (volatile unsigned short*) 0x400000e; break;
    }

    /* set the appropriate bits in the control register */
    *control = priority |
               (0 << 2) |            // charblock [0-3]
               (0 << 6) |            // mosaic flag [0-1]
               (1 << 7) |            // color mode [0-1] 0 is 16 colors, 1 is 256
               (screenblock << 8) |  // screenblock [0-31]
               (1 << 13) |           // wrapping flag [0-1]
               (0 << 14);            // bg size [0-3] 0 is 256x256
}

/* write text to a specific tile on the screen */
void set_text(char* str, int row, int col, unsigned short* textmap) {
    /* find the index in the texmap to draw to */
    int index = row * 32 + col;

    /* the text characters start down in the image */
    int base = 32 * 11;

    /* the first 32 characters are missing from the map (controls etc.) */
    int missing = 32;

    /* for each character */
    while (*str) {

        /* place this character in the map */
        textmap[index] = base + (*str - missing);

        /* move onto the next character */
        index++;
        str++;
    }
}



