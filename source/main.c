/*
 * main.c
 * this file contains the main function for the game
 */

#include "gba.h"
#include "moonpatrol.h"

/* the main function */
int main( ) { 
    /* we set the mode to mode 0 */
    *REG_DISPCNT = MODE_0;

    /* declare loop variables (declarations inside for statement are
     * NOT allowed in ANSI C) */
    int row, col;

    /* loop through each column of the screen */
    for (col = 0; col < WIDTH; col++) {

        /* make a color in the range of black to bright red based on the row */
        unsigned short color = make_color(col % 32, 0, 0);

        /* loop through each column of the screen */
        for (row = 0; row < HEIGHT; row++) { 
            /* set the screen location to this color */
            SCREEN[col + row * WIDTH] = color;
        }
    }

    /* we now loop forever displaying the image */
    while (1) {
        /* do nothing */
    }
}

