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

/* wait for a vertical refresh */
void wait_vblank(void) {
    while ((REG_DISPSTAT & 1)) {
        /* do nothing */
    }
}

/* delay for an amount of time */
void delay(int amount) {
    int i;
    for (i = 0; i < amount * 10; i++) {

    }
}


