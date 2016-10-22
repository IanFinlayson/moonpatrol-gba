/*
 * graphics.c
 * this file contains graphics routines for the GBA in general such as
 * dealing with background layers and sprite objects
 */

#include "gba.h"
#include "moonpatrol.h"

/* initialize a background */
void init_background(int bg, int priority, int scrnblk) {
    /* get the appropriate control register */
    volatile unsigned short* control;
    switch (bg) {
        case 0: control = (volatile unsigned short*) 0x4000008; break;
        case 1: control = (volatile unsigned short*) 0x400000a; break;
        case 2: control = (volatile unsigned short*) 0x400000c; break;
        case 3: control = (volatile unsigned short*) 0x400000e; break;
    }

    /* set the appropriate bits in the control register */
    *control = priority |
               (0 << 2) |       /* charblock [0-3] */
               (0 << 6) |       /* mosaic flag [0-1] */
               (1 << 7) |       /* color mode, 0 is 16 colors, 1 is 256 */
               (scrnblk << 8) | /* screenblock [0-31] */
               (1 << 13) |      /* wrapping flag [0-1] */
               (0 << 14);       /* bg size [0-3] 0 is 256x256 */
}

/* write text to a specific tile on the screen */
void set_text(char* str, int row, int col, unsigned short* textmap) {
    /* find the index in the texmap to draw to */
    int index = row * 32 + col;

    /* the text characters start down in the image */
    int base = 32 * 11;

    /* the first 32 characters are missing from the map (controls etc.) */
    int missing = 32;

    /* for each character */
    while (*str) {
        /* place this character in the map */
        textmap[index] = base + (*str - missing);

        /* move onto the next character */
        index++;
        str++;
    }
}

/* array of all of the 128 sprite objects we have */
struct Sprite sprites[128];

/* the next available sprite index */
int next_sprite_index = 0;

/* function to initialize a sprite with its properties, and return a pointer */
struct Sprite* sprite_init(int x, int y, enum SpriteSize size,
        int horizontal_flip, int vertical_flip, int tile_index, int priority) {
    /* grab the next index */
    int index = next_sprite_index++;

    /* setup the bits used for each shape/size possible */
    int size_bits, shape_bits;
    switch (size) {
        case SIZE_8_8:   size_bits = 0; shape_bits = 0; break;
        case SIZE_16_16: size_bits = 1; shape_bits = 0; break;
        case SIZE_32_32: size_bits = 2; shape_bits = 0; break;
        case SIZE_64_64: size_bits = 3; shape_bits = 0; break;
        case SIZE_16_8:  size_bits = 0; shape_bits = 1; break;
        case SIZE_32_8:  size_bits = 1; shape_bits = 1; break;
        case SIZE_32_16: size_bits = 2; shape_bits = 1; break;
        case SIZE_64_32: size_bits = 3; shape_bits = 1; break;
        case SIZE_8_16:  size_bits = 0; shape_bits = 2; break;
        case SIZE_8_32:  size_bits = 1; shape_bits = 2; break;
        case SIZE_16_32: size_bits = 2; shape_bits = 2; break;
        case SIZE_32_64: size_bits = 3; shape_bits = 2; break;
    }

    int h = horizontal_flip ? 1 : 0;
    int v = vertical_flip ? 1 : 0;

    /* set up the first attribute */ 
    sprites[index].attribute0 = y |             /* y coordinate */
                            (0 << 8) |          /* rendering mode */
                            (0 << 10) |         /* gfx mode */
                            (0 << 12) |         /* mosaic */
                            (1 << 13) |         /* color mode, 0:16, 1:256 */
                            (shape_bits << 14); /* shape */
                                                                                
    /* set up the second attribute */ 
    sprites[index].attribute1 = x |             /* x coordinate */
                            (0 << 9) |          /* affine flag */
                            (h << 12) |         /* horizontal flip flag */
                            (v << 13) |         /* vertical flip flag */
                            (size_bits << 14);  /* size */
                                                                                
    /* setup the second attribute */ 
    sprites[index].attribute2 = tile_index |   // tile index */
                            (priority << 10) | // priority */
                            (0 << 12);         // palette bank (only 16 color)*/

    /* return pointer to this sprite */
    return &sprites[index];
}

/* update all of the spries on the screen */
void sprite_update_all() {
    /* point a short* into the array of structs */
    unsigned short* temp = (unsigned short*) sprites; 

    /* copy them all over */
    for(int i = 0; i < 128 * 4; i++) {
        OBJECT_ATTRIBUTE_MEMOORY[i] = temp[i]; 
    }
}

/* setup all sprites */
void sprite_clear() {
    /* clear the index counter */
    next_sprite_index = 0;

    /* move all sprites offscreen to hide them */
    for(int i = 0; i < 128; i++) {
        sprites[i].attribute0 = SCREEN_HEIGHT;
        sprites[i].attribute1 = SCREEN_WIDTH;
    }
}

/* move a sprite in a direction */
void sprite_move(struct Sprite* sprite, int dx, int dy) {
    /* get the current y coordinate */
    int y = sprite->attribute0 & 0xff;

    /* get the current x coordinate */
    int x = sprite->attribute1 & 0x1ff;

    /* move to the new location */
    sprite_position(sprite, x + dx, y + dy);
}

/* set a sprite postion */
void sprite_position(struct Sprite* sprite, int x, int y) {
    /* clear out the y coordinate */
    sprite->attribute0 &= 0xff00;

    /* set the new y coordinate */
    sprite->attribute0 |= (y & 0xff);

    /* clear out the x coordinate */
    sprite->attribute1 &= 0xfe00;

    /* set the new x coordinate */
    sprite->attribute1 |= (x & 0x1ff);
}

/* change the tile offset of a sprite */
void sprite_set_offset(struct Sprite* sprite, int offset) {
    /* clear the old offset */
    sprite->attribute2 &= 0xfc00;

    /* apply the new one */
    sprite->attribute2 |= (offset & 0x03ff);
}

