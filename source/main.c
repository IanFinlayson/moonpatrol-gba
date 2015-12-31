/*
 * main.c
 * this file contains the main function for the game
 */

#include "gba.h"
#include "moonpatrol.h"
#include "../images/layer0.h"
#include "../maps/layer0_map.h"

/* the main function */
int main( ) {
    /* we set the mode to mode 0 with background 0 turned on*/
    *REG_DISPCNT = MODE_0 | BG0_ENABLE;

    /* set up background 0 */
    REG_BG0_PTR = BG_COLOR256 | TEXTBG_SIZE_256x256 |
        (31 << SCREEN_SHIFT) | WRAPAROUND;

    /* load the palette into background palette memory */
    dma_memcpy((void*) layer0_palette, (void*) BG_PALETTE_MEMORY,
            256, DMA_16_NOW);

    /* load the tile image into tile memory */
    dma_memcpy((void*) layer0_data, (void*) CharBaseBlock(0),
            (layer0_width * layer0_height), DMA_16_NOW);

    /* copy the tile map itself into memory */
    unsigned short* bg0map =(unsigned short*)ScreenBaseBlock(31);
    dma_memcpy((void*)layer0_map, (void*)bg0map, 1024, DMA_32_NOW);



    /* we now loop forever displaying the image */
    while (1) {
        /* do nothing */
    }
}

