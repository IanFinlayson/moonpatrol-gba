/*
 * graphics.c
 * this file contains graphics routines
 */

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

