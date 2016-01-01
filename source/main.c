/*
 * main.c
 * this file contains the main function for the game
 */

#include "gba.h"
#include "moonpatrol.h"
#include "../images/background.h"
#include "../maps/layer0.h"
#include "../maps/layer1.h"

/* setup the background images */
void setup_backgrounds() {
    /* load the palette into background palette memory */
    dma_memcpy((void*) background_palette, (void*) BG_PALETTE_MEMORY,
            256, DMA_16_NOW);

    /* load the tile image into tile memory */
    dma_memcpy((void*) background_data, (void*) CharBaseBlock(0),
            (background_width * background_height), DMA_16_NOW);

    /* set the control register for bg 0 */
    REG_BG0_CONTROL = 2 |                   // priority [0-3] 0 is highest
                      (0 << 2) |            // charblock [0-3]
                      (0 << 6) |            // mosaic flag [0-1]
                      (1 << 7) |            // color mode [0-1] 0 is 16 colors, 1 is 256
                      (20 << 8) |            // screenblock [0-31]
                      (1 << 13) |           // wrapping flag [0-1]
                      (0 << 14);            // bg size [0-3] (see docs)

    /* copy tile map 0 into memory */
    unsigned short* bg0map = (unsigned short*)ScreenBaseBlock(20);
    dma_memcpy((void*)layer0, (void*)bg0map, 1024, DMA_32_NOW);


    /* set bg 1 flags */
    REG_BG1_CONTROL = 0 |                   // priority [0-3] 0 is highest
                      (0 << 2) |            // charblock [0-3]
                      (0 << 6) |            // mosaic flag [0-1]
                      (1 << 7) |            // color mode [0-1] 0 is 16 colors, 1 is 256
                      (21 << 8) |            // screenblock [0-31]
                      (1 << 13) |           // wrapping flag [0-1]
                      (0 << 14);            // bg size [0-3] (see docs)

    /* copy tile map 1 into memory */
    unsigned short* bg1map = (unsigned short*)ScreenBaseBlock(21);
    dma_memcpy((void*)layer1, (void*)bg1map, 1024, DMA_32_NOW);



}

/* the main function */
int main( ) {
    /* we set the mode to mode 0 with backgrounds 0 and 1 turned on*/
    *REG_DISPCNT = MODE_0 | BG0_ENABLE | BG1_ENABLE;

    /* set up the backgrounds */
    setup_backgrounds();

    int x = 0, y = 0;
    /* we now loop forever displaying the image */
    while (1) {
        /* wait for vertical refresh */
		wait_vblank();

        /* for now the buttons just scroll the background */
		if (button_down(BUTTON_LEFT)) x--;
		if (button_down(BUTTON_RIGHT)) x++;
		if (button_down(BUTTON_UP)) y--;
		if (button_down(BUTTON_DOWN)) y++;

        /* assign the variables into the background scroll registers */
		REG_BG0_XSCROLL = x;
		REG_BG0_YSCROLL = y;
		REG_BG1_XSCROLL = x*2;
		REG_BG1_YSCROLL = y*2;

        /* wait for vertical refresh again */
		wait_vblank();

        /* delay for a little bit */
        delay(1200);
    }
}

