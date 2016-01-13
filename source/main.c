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

/* the moon rover */
struct Rover {
    int side;
    struct Sprite* body;
    struct Sprite* wheels[3];
};

/* setup the rover */
void init_rover(struct Rover* rover) {
    /* setup our sprites */
    rover->body = setup_sprite(0, 104, 125, SIZE_32_16, 0, 0, 0, 1);
    int i;
    for (i = 0; i < 3; i++) {
        rover->wheels[i] = setup_sprite(1 + i, 104 + 12*i, 135, SIZE_8_8, 0, 0, 32, 0);
    }
    rover->side = 0;
}

/* move the rover left or right */
void rover_left(struct Rover* rover) {
    move_sprite(rover->body, -ROVER_SPEED, 0);
    int i;
    for (i = 0; i < 3; i++) {
        move_sprite(rover->wheels[i], -ROVER_SPEED, 0);
    }
}
void rover_right(struct Rover* rover) {
    move_sprite(rover->body, ROVER_SPEED, 0);
    int i;
    for (i = 0; i < 3; i++) {
        move_sprite(rover->wheels[i], ROVER_SPEED, 0);
    }
}

/* flip the wheel animation in the rover */
void rover_flip(struct Rover* rover) {
    int offset, i;

    if (rover->side) {
        offset = 34;
        rover->side = 0;
    } else {
        offset = 32;
        rover->side = 1;
    }

    for (i = 0; i < 3; i++) {
        sprite_set_offset(rover->wheels[i], offset);
    }

}


/* the sprite objects we have */
struct Sprite* ship;

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
    unsigned short* bg3map = (unsigned short*)ScreenBaseBlock(19);
    int i;
    /* zero out the text layer */
    for (i = 0; i < 1024*2; i++) {
        bg3map[i] = 0;
    }


    /* FIXME temporary text */
    set_text("Hello!", 0, 0, bg3map);
}


/* setup the sprite objects in memory */
void setup_sprites() {
    /* clear all sprites out */
    init_sprites();

    /* load the palette into object palette memory */
    dma_memcpy((void*) objects_palette, (void*) OBJECT_PALETTE_MEMORY,
            256, DMA_16_NOW);

    /* load the object image into memory */
    dma_memcpy((void*) objects_data, (void*) OBJECT_DATA_MEMORY,
            (objects_width * objects_height) / 2, DMA_16_NOW);


    /* setup our space ship */
    ship = setup_sprite(4, 50, 50, SIZE_32_16, 0, 0, 16, 0);
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

    /* scroll the bgs up a bit (rather than adjust the maps) */
    REG_BG0VOFS = 5;
    REG_BG1VOFS = 20;
    REG_BG2VOFS = 5;

    /* the rover */
    struct Rover rover;
    init_rover(&rover);

    /* the scroll for the game */
    int x = 0;

    /* we now loop forever displaying the image */
    while (1) {
        /* wait for vertical refresh */
		wait_vblank();

        /* we scroll right continuously in this game */
        x++;

        rover_flip(&rover);

        /* move the rover on user input */
        if (button_down(BUTTON_LEFT)) {
            rover_left(&rover);
        }
        if (button_down(BUTTON_RIGHT)) {
            rover_right(&rover);
        }

        /* parallax it up */
        REG_BG0HOFS = x;
        REG_BG1HOFS = x << 1;
        REG_BG2HOFS = x << 2;

        /* wait for vertical refresh again */
        wait_vblank();

        /* update all sprites on screen */
        update_sprites(); 

        /* delay for a little bit */
        delay(1800);
    }
}

