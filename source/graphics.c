/*
 * graphics.c
 * this file contains graphics routines
 */

#include "gba.h"

/* function to compute a 16-bit integer color based on the three components
 * the first bit is ignored, then there are five bits for blue, 5 bits for
 * green and five bits for red, valid parameters are in the range of 0-31
 */ 
unsigned short make_color(unsigned char r, unsigned char g, unsigned char b) {
    unsigned short color = b << 10;
    color += g << 5;
    color += r;
    return color;
}

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



