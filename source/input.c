/*
 * input.c
 * this file contains input routines
 */

#include "gba.h"

/* check if a button is down */
int button_down(int button) {
    if (!(BUTTONS & button)) {
        return 1;
    } else {
        return 0;
    }
}

