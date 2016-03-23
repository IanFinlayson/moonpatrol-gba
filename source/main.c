/*
 * main.c
 * this file contains the main function for the game
 */

#include "gba.h"
#include "moonpatrol.h"
#include "../images/background.h"
#include "../images/objects.h"
#include "../maps/layer0.h"
#include "../maps/layer1.h"
#include "../maps/layer2.h"

/* control the paralax effect of the layers, and ground speed which affects the
 * gameplay a lot - these are rshift amounts, so larger means slower */
#define SCROLL_MOUNTAINS 5
#define SCROLL_HILLS 4
#define SCROLL_GROUND 2

/* the sprite objects we have */
struct Sprite* ship;
unsigned short* bg3map;
/* setup the background images */
void setup_backgrounds() {
    /* load the palette into background palette memory */
    dma_memcpy((void*) background_palette, (void*) BG_PALETTE_MEMORY,
            256, DMA_16_NOW);

    /* load the tile image into memory */
    dma_memcpy((void*) background_data, (void*) CharBaseBlock(0),
            (background_width * background_height) / 2, DMA_16_NOW);

    /* set up layer 0 (mountains) */
    init_background(0, 3, 16);
    unsigned short* bg0map = (unsigned short*)ScreenBaseBlock(16);
    dma_memcpy((void*)layer0, (void*)bg0map, 1024, DMA_32_NOW);

    /* set up layer 1 (hills) */
    init_background(1, 2, 17);
    unsigned short* bg1map = (unsigned short*)ScreenBaseBlock(17);
    dma_memcpy((void*)layer1, (void*)bg1map, 1024, DMA_32_NOW);

    /* set up layer 2 (ground) */
    init_background(2, 1, 18);
    unsigned short* bg2map = (unsigned short*)ScreenBaseBlock(18);
    dma_memcpy((void*)layer2, (void*)bg2map, 1024, DMA_32_NOW);

    /* set up layer 3 (ui) */
    init_background(3, 0, 19);
    bg3map = (unsigned short*)ScreenBaseBlock(19);
    /* zero out the text layer */
    for (int i = 0; i < 1024*2; i++) {
        bg3map[i] = 0;
    }


    /* FIXME temporary text */
    //set_text("Hello!", 0, 0, bg3map);
}


/* setup the sprite objects in memory */
void setup_sprites() {
    /* clear all sprites out */
    sprite_clear();

    /* load the palette into object palette memory */
    dma_memcpy((void*) objects_palette, (void*) OBJECT_PALETTE_MEMORY,
            256, DMA_16_NOW);

    /* load the object image into memory */
    dma_memcpy((void*) objects_data, (void*) OBJECT_DATA_MEMORY,
            (objects_width * objects_height) / 2, DMA_16_NOW);


    /* setup our space ship */
    ship = sprite_init(50, 50, SIZE_32_16, 0, 0, 16, 0);
}



/* the main function */
int main( ) {
    /* we set the mode to mode 0 with all backgrounds and objects turned on */
    *REG_DISPCNT = MODE_0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE |
        OBJ_ENABLE | OBJ_MAP_1D;

    /* set up the backgrounds */
    setup_backgrounds();

    /* setup the sprites */
    setup_sprites();

    /* setup the obstacles */
    obstacles_init();

    /* scroll the bgs up a bit (rather than adjust the maps) */
    REG_BG0VOFS = 5;
    REG_BG1VOFS = 20;
    REG_BG2VOFS = 5;

    /* the rover */
    struct Rover rover;
    rover_init(&rover);

    /* the scroll for the game */
    unsigned int scroll = 0;
    int scroll_speed = SCROLL_MEDIUM;

    /* we now loop forever displaying the image */
    while (1) {
        /* wait for vertical refresh */
		wait_vblank();

        /* we scroll right continuously in this game */
        scroll += scroll_speed;

        /* move the rover on user input */
        if (button_down(BUTTON_LEFT)) {
            scroll_speed = SCROLL_SLOW;
            rover_left(&rover);
        }
        else if (button_down(BUTTON_RIGHT)) {
            scroll_speed = SCROLL_FAST;
            rover_right(&rover);
        } else {
            scroll_speed = SCROLL_MEDIUM;
        }

        /* jump when the A button is pressed */
        if (button_down(BUTTON_A)) {
            rover_jump(&rover);
        }

        /* parallax it up */
        REG_BG0HOFS = scroll >> SCROLL_MOUNTAINS;
        REG_BG1HOFS = scroll >> SCROLL_HILLS;
        REG_BG2HOFS = scroll >> SCROLL_GROUND;

        /* update the obstacle positions */
        obstacles_update(scroll >> SCROLL_GROUND);

        /* update the rover position */
        rover_update(&rover, scroll >> SCROLL_GROUND);

        /* wait for vertical refresh again */
        wait_vblank();

        /* update all sprites on screen */
        sprite_update_all();

        /* delay for a little bit */
        delay(250);
    }
}

