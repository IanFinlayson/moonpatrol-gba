/*
 * hello.c
 * this program is a simple GBA example
 * it simply creates an image and waits
 */

/* the width and height of the screen */
#define WIDTH 240
#define HEIGHT 160


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

/* the main function */
int main( ) { 
    /* the screen is simply a pointer into memory at a specific
     * address this pointer points to 16-bit colors of which there
     * are 240x160 */
    unsigned short* screen = (unsigned short*) 0x6000000;

    /* the mode pointer points to a the gba graphics register */
    unsigned long* mode = (unsigned long*) 0x4000000;

    /* we set the mode to mode 3 */
    *mode = 0x403;

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
            screen[col + row * WIDTH] = color;
        }
    }

    /* we now loop forever displaying the image */
    while (1) {
        /* do nothing */
    }
}

