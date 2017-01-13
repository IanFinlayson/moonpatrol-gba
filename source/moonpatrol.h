/*
 * moonpatrol.h
 * this file contains function and type declarations for the moon patrol game
 */

#ifndef MOONPATROL_H
#define MOONPATROL_H

/* the scroll speeds */
#define SCROLL_SLOW 3
#define SCROLL_MEDIUM 4
#define SCROLL_FAST 5

/* the shift amount to convert pixels to/from pixels/256 */
#define VERT_SHIFT_AMOUNT 8

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
void set_text(char* str, int row, int col);

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
struct Sprite* sprite_init(int x, int y, enum SpriteSize size,
        int horizontal_flip, int vertical_flip, int tile_index, int priority);

/* update all sprite positions on screen - should be drawn after refresh */
void sprite_update_all();

/* clear all sprites off the screen */
void sprite_clear();

/* move a sprite in a direction */
void sprite_move(struct Sprite* sprite, int dx, int dy);

/* set a sprite postion */
void sprite_position(struct Sprite* sprite, int x, int y);

/* set a sprite tile offset */
void sprite_set_offset(struct Sprite* sprite, int offset);

/* the moon rover */
struct Rover {
    /* sprites used for the body and wheels */
    struct Sprite* body;
    struct Sprite* wheels[3];

    /* which side of the animation is displayed */
    int side;

    /* counter used to control the animation */
    int anim_counter;

    /* counter used to move the rover */
    int move_counter;

    /* the x and y position of the rover
     * the y, and the dy, and wheel_height below are actually measured in 1/256
     * pixels.  This allows for fixed point math to implement our jumping
     * mechanics */
    int x, y;

    /* the y speed of the rover and whether we are jumping */
    int dy;
    int jumping;

    /* the height of each wheel */
    int wheel_height[3];
};

/* initialize the rover */
void rover_init(struct Rover* rover);

/* move the rover left or right */
void rover_left(struct Rover* rover);
void rover_right(struct Rover* rover);

/* jump the rover into the air */
void rover_jump(struct Rover* rover);

/* update the rover position and animation */
void rover_update(struct Rover* rover, int scroll);

/* perform the crash animation on the rover */
void rover_crash(struct Rover* rover);

struct Ship {
    struct Sprite* sprite;

    /* the x and y position of the rover
     * the y, and the dy, and wheel_height below are actually measured in 1/256
     * pixels.  This allows for fixed point math to implement our jumping
     * mechanics */
    int x, y;

    /* the index to use when updating the ship along predefined paths */
    int movement_counter;

    /* counter to keep track of how many frames since last update */
    int update_frames;
};

/* initialize the rover */
void ship_init(struct Ship* ship);

/* update the ship position and animation */
void ship_update(struct Ship* ship, int scroll);

/* represents one obstacle, either a pit or a mound */
struct Obstacle {
    int start_x, x, y;
    int width;
    struct Sprite* sprite;
};

/* initialize the obstacles */
void obstacles_init();

/* update the obstacles */
void obstacles_update(int scroll);

/* check if the rover has crashed into any of the obstacles */
int obstacles_crash(struct Rover* rover, int scroll);

#endif

