/*
 * main.c
 * this file contains the main function for the game
 */

#include <stdio.h>

#include "gba.h"
#include "moonpatrol.h"
#include "../images/background.h"
#include "../images/objects.h"
#include "../maps/layer0.h"
#include "../maps/layer1.h"
#include "../maps/layer2.h"
#include "../audio/music_16K_mono.h"

/* control the paralax effect of the layers, and ground speed which affects the
 * gameplay a lot - these are rshift amounts, so larger means slower */
#define SCROLL_MOUNTAINS 5
#define SCROLL_HILLS 4
#define SCROLL_GROUND 2

/* the sprite objects we have */
/* setup the background images */
void setup_backgrounds() {
    int x = 4;

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
    /* zero out the text layer */
    for (int i = 0; i < 1024*2; i++) {
        bg3map[i] = 0;
    }
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
}

/* start the music playing and init the sound system */
void setup_sounds() {
    INTERRUPT_ENABLE = 0;
    INTERRUPT_CALLBACK = (unsigned int) &on_vblank;
    INTERRUPT_SELECTION |= INTERRUPT_VBLANK;
    DISPLAY_INTERRUPTS |= 0x08;
    INTERRUPT_ENABLE = 1;

    /* clear the sound control initially */
    SOUND_CONTROL = 0;

    /* set the music to play on channel A */
    play_sound(music_16K_mono, music_16K_mono_bytes, 16000, 'A');
}

/* handle the current score on crash */
void handle_score(int score, int* high_score) {
    if (score > *high_score) {
        *high_score = score;
    }
}

/* the main function */
int main() {
    /* we set the mode to mode 0 with all backgrounds and objects turned on */
    *REG_DISPCNT = MODE_0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE |
        OBJ_ENABLE | OBJ_MAP_1D;

    /* load the current high score from ROM (byte-by-byte) */
    int high_score = 0; 

top:
    /* set up the backgrounds */
    setup_backgrounds();

    /* setup the sprites */
    setup_sprites();

    /* setup sounds */
    setup_sounds();

    /* setup the obstacles */
    obstacles_init();

    /* scroll the bgs up a bit (rather than adjust the maps) */
    REG_BG0VOFS = 5;
    REG_BG1VOFS = 20;
    REG_BG2VOFS = 5;

    /* the rover */
    struct Rover rover;
    rover_init(&rover);

    /* the enemy ship */
    struct Ship ship;
    ship_init(&ship);

    /* the scroll for the game */
    unsigned int scroll = 0;
    int scroll_speed = SCROLL_MEDIUM;

    /* the frame is used for calculating the scrore */
    int frame = 0;
    int bonus = 0;
    set_text("Score: 0", 0, 0);
    char score_string[32];
    sprintf(score_string, "High Score: %d", high_score);
    set_text(score_string, 0, 14);

    /* we now loop forever doing the game */
    while (1) {
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

        /* fire when the B button is pressed */
        if (button_down(BUTTON_B)) {
            rover_fire(&rover);
        }

        /* update the obstacle positions */
        obstacles_update(scroll >> SCROLL_GROUND);

        /* update the rover position */
        rover_update(&rover, &ship, scroll >> SCROLL_GROUND, &bonus);

        /* check if the rover has crashed into an obstacle */
        if (obstacles_crash(&rover, scroll >> SCROLL_GROUND)) {
            rover_crash(&rover);
            wait_vblank();
            sprite_update_all();
            handle_score((frame >> 8) * 10 + bonus, &high_score);
            delay(250);
            goto top;
        }

        /* update the enemy ship, returns if the ship has destroyed the rover */
        if (ship_update(&ship, scroll >> SCROLL_GROUND, &rover)) {
            rover_crash(&rover);
            wait_vblank();
            sprite_update_all();
            handle_score((frame >> 8) * 10 + bonus, &high_score);
            delay(250);
            goto top;
        }

        /* update the score every 256 frames to be 1/256 frame * 10 */
        frame++;
        if ((frame & 0xff) == 0) {
            char score_string[32];
            sprintf(score_string, "Score: %d", (frame >> 8) * 10 + bonus);
            set_text(score_string, 0, 0);
            sprintf(score_string, "High Score: %d", high_score);
            set_text(score_string, 0, 14);
        }

        /* wait for vertical refresh again */
        wait_vblank();

        /* update the scroll registers */
        REG_BG0HOFS = scroll >> SCROLL_MOUNTAINS;
        REG_BG1HOFS = scroll >> SCROLL_HILLS;
        REG_BG2HOFS = scroll >> SCROLL_GROUND;

        /* update all sprites on screen */
        sprite_update_all();

        /* delay for a little bit */
        delay(250);
    }
}

