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




//misc sprite constants
#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40
#define OBJ_ENABLE 0x1000

//attribute0 stuff
#define ROTATION_FLAG 0x100
#define SIZE_DOUBLE 0x200
#define MODE_NORMAL 0x0
#define MODE_TRANSPERANT 0x400
#define MODE_WINDOWED 0x800
#define MOSAIC 0x1000
#define COLOR_16 0x0000
#define COLOR_256 0x2000
#define SQUARE 0x0
#define TALL 0x4000
#define WIDE 0x8000

//attribute1 stuff
#define ROTDATA(n) ((n) << 9)
#define HORIZONTAL_FLIP 0x1000
#define VERTICAL_FLIP 0x2000
#define SIZE_8 0x0
#define SIZE_16 0x4000
#define SIZE_32 0x8000
#define SIZE_64 0xC000

//attribute2 stuff
#define PRIORITY(n) ((n) << 10)
#define PALETTE(n) ((n) << 12)

//sprite structs
typedef struct tagSprite
{
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
}Sprite,*pSprite;

typedef struct tagSpriteRotate
{
    unsigned short filler1[3];
    unsigned short pa;
    unsigned short filler2[3];
    unsigned short pb;
    unsigned short filler3[3];
    unsigned short pc;
    unsigned short filler4[3];
    unsigned short pd;
}SpriteRotate,*pSpriteRotate;

//create an array of 128 sprites equal to OAM
Sprite sprites[128];

//create pointer to the same sprite array for rotations
pSpriteRotate rotData = (pSpriteRotate)sprites;




/* setup the sprite objects in memory */
void setup_sprites() {
    /* load the palette into object palette memory */
    dma_memcpy((void*) objects_palette, (void*) OBJECT_PALETTE_MEMORY,
            256, DMA_16_NOW);

    /* load the object image into memory */
    dma_memcpy((void*) objects_data, (void*) OBJECT_DATA_MEMORY,
            (objects_width * objects_height) / 2, DMA_16_NOW);

    /* move all sprites offscreen to hide them */
    int n;
    for(n = 0; n < 128; n++) {
        sprites[n].attribute0 = 160;
        sprites[n].attribute1 = 240;
    }

    /* set up the first attribute */
    sprites[0].attribute0 = 125 |           // y coordinate
                            (0 << 8) |    // rendering mode
                            (0 << 10) |   // gfx mode
                            (0 << 12) |   // mosaic
                            (1 << 13) |   // color mode, 0 is 16 colors, 1 is 256
                            (1 << 14);    // size

    /* set up the second attribute */
    sprites[0].attribute1 = 104 |           // x coordinate
                            (0 << 9) |    // affine flag
                            (0 << 12) |   // horizontal flip flag
                            (0 << 13) |   // vertical flip flag
                            (2 << 14);    // shape

    /* setup the second attribute */
    sprites[0].attribute2 = 0 |           // tile index?
                            (0 << 10) |   // priority 0 is low!
                            (0 << 12);    // palette bank (only in 16 color mode)


// now the other one!!!!!!!!!!1



    /* set up the first attribute */
    sprites[1].attribute0 = 50 |         // y coordinate
                            (0 << 8) |    // rendering mode
                            (0 << 10) |   // gfx mode
                            (0 << 12) |   // mosaic
                            (1 << 13) |   // color mode, 0 is 16 colors, 1 is 256
                            (1 << 14);    // size

    /* set up the second attribute */
    sprites[1].attribute1 = 50 |          // x coordinate
                            (0 << 9) |    // affine flag
                            (0 << 12) |   // horizontal flip flag
                            (0 << 13) |   // vertical flip flag
                            (2 << 14);    // shape

    /* setup the second attribute */
    sprites[1].attribute2 = 16 |           // tile index?
                            (0 << 10) |   // priority 0 is low!
                            (0 << 12);    // palette bank (only in 16 color mode)
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

    int x = 0;
    /* we now loop forever displaying the image */
    while (1) {
        /* wait for vertical refresh */
		wait_vblank();

        /* we scroll right continuously in this game */
        x++;

        /* parallax it up */
        REG_BG0HOFS = x;
        REG_BG1HOFS = x << 1;
        REG_BG2HOFS = x << 2;

        /* wait for vertical refresh again */
        wait_vblank();


        int n; 
        unsigned short* temp; 
        temp = (unsigned short*)sprites; 

        for(n = 0; n < 128*4; n++) {
            OBJECT_ATTRIBUTE_MEMOORY[n] = temp[n]; 

        }


        /* delay for a little bit */
        delay(2000);
    }
}

