/*
 * main.c
 * this file contains the main function for the game
 */

#include "gba.h"
#include "moonpatrol.h"
#include "../images/layer0.h"
#include "../maps/layer0_map.h"


//button identifiers
#define BUTTON_RIGHT 16
#define BUTTON_LEFT  32
#define BUTTON_UP    64
#define BUTTON_DOWN  128
#define BUTTONS (*(volatile unsigned int*)0x04000130)

//scrolling registers for background 0
#define REG_BG0HOFS *(volatile unsigned short*)0x4000010
#define REG_BG0VOFS *(volatile unsigned short*)0x4000012

//vertical refresh register
#define REG_DISPSTAT   *(volatile unsigned short*)0x4000004


//wait for vertical refresh
void WaitVBlank(void)
{
    while((REG_DISPSTAT & 1));
}



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


    int x = 0, y = 0;
    /* we now loop forever displaying the image */
    while (1) {
        //wait for vertical refresh
		WaitVBlank();

		//D-pad moves background
		if(!(BUTTONS & BUTTON_LEFT)) x--;
		if(!(BUTTONS & BUTTON_RIGHT)) x++;
		if(!(BUTTONS & BUTTON_UP)) y--;
		if(!(BUTTONS & BUTTON_DOWN)) y++;

		//use hardware background scrolling
		REG_BG0VOFS = y ;
		REG_BG0HOFS = x ;

        //wait for vertical refresh
		WaitVBlank();
		
        int n;
		for(n = 0; n < 4000; n++);
    }
}

