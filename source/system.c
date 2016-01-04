/*
 * system.c
 * this file contains functions relating to the GBA system/hardware
 */

#include "gba.h"

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

/* check if a button is down */
int button_down(int button) {
    if (!(BUTTONS & button)) {
        return 1;
    } else {
        return 0;
    }
}

