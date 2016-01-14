/*
 * moonpatrol.h
 * this file contains function and type declarations for the moon patrol game
 */

#ifndef MOONPATROL_H
#define MOONPATROL_H

/* the speed of the rover in pixels per frame */
#define ROVER_SPEED 1

/* frames in between rover wheel flips */
#define ROVER_WHEEL_FLIP 15

/* wait for a vertical refresh */
void wait_vblank(void);

/* delay for an amount of time */
void delay(int amount);

/* check if a button is down */
int button_down(int button);

/* initialize a background */
void init_background(int bg, int priority, int screenblock);

/* perform fast DMA memory copies */
void dma_memcpy(void* source, void* dest, unsigned count, unsigned mode);

/* write text to a specific tile on the screen */
void set_text(char* str, int row, int col, unsigned short* textmap);

/* a sprite representing a hardware sprite object */
struct Sprite {
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
};

/* the different sizes of sprites which are possible */
enum SpriteSize {
    SIZE_8_8,
    SIZE_16_16,
    SIZE_32_32,
    SIZE_64_64,
    SIZE_16_8,
    SIZE_32_8,
    SIZE_32_16,
    SIZE_64_32,
    SIZE_8_16,
    SIZE_8_32,
    SIZE_16_32,
    SIZE_32_64
};

/* function to initialize a sprite with its properties, and return a pointer */
struct Sprite* setup_sprite(int index, int x, int y, enum SpriteSize size,
        int horizontal_flip, int vertical_flip, int tile_index, int priority);

/* update all sprite positions on screen - should be drawn after refresh */
void update_sprites();

/* initialize all sprites */
void init_sprites();

/* move a sprite in a direction */
void move_sprite(struct Sprite* sprite, int dx, int dy);

/* set a sprite postion */
void position_sprite(struct Sprite* sprite, int x, int y);

/* set a sprite tile offset */
void sprite_set_offset(struct Sprite* sprite, int offset);

/* the moon rover */
struct Rover {
    int side;
    int counter;
    struct Sprite* body;
    struct Sprite* wheels[3];
};

/* initialize the rover */
void rover_init(struct Rover* rover);

/* move the rover left or right */
void rover_left(struct Rover* rover);
void rover_right(struct Rover* rover);

/* flip the wheel animation in the rover */
void rover_flip(struct Rover* rover);

#endif

