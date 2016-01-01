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

    /* set bg 0 flags */
    REG_BG0_CONTROL = BG_COLOR256 | TEXTBG_SIZE_256x256 |
        (31 << SCREEN_SHIFT) | WRAPAROUND;

    /* copy tile map 0 into memory */
    unsigned short* bg0map = (unsigned short*)ScreenBaseBlock(31);
    dma_memcpy((void*)layer0, (void*)bg0map, 1024, DMA_32_NOW);




#if 0
    /* set bg 1 flags */
    REG_BG1_CONTROL = BG_COLOR256 | TEXTBG_SIZE_256x256 |
        (31 << SCREEN_SHIFT) | WRAPAROUND;

    /* copy tile map 1 into memory */
    unsigned short* bg1map = (unsigned short*)ScreenBaseBlock(31) + (1024 * 32);
    dma_memcpy((void*)layer1, (void*)bg1map, 1024, DMA_32_NOW);
#endif



}

/* the main function */
int main( ) {
    /* we set the mode to mode 0 with backgrounds 0 and 1 turned on*/
    *REG_DISPCNT = MODE_0 | BG0_ENABLE                  ;//| BG1_ENABLE;

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
		REG_BG1_XSCROLL = x;
		REG_BG1_YSCROLL = y;

        /* wait for vertical refresh again */
		wait_vblank();
		
        /* delay for a little bit */
        delay(400);
    }
}

