/*
 * system.c
 * this file contains functions relating to the GBA system/hardware
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
    for (int i = 0; i < amount * 10; i++) {

    }
}

/* check if a button is down */
int button_down(int button) {
    if (!(BUTTONS & button)) {
        return 1;
    } else {
        return 0;
    }
}

