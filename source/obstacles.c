/* obstacles.c
 * functions related to the two types of obstacles: pits and mounds */

#include <stdlib.h>

#include "gba.h"
#include "moonpatrol.h"

/* how many obstacles there are */
#define NUM_OBSTACLES 6

/* offset into the image file */
#define PIT_OFFSET 36
#define MOUND_OFFSET 44

/*  get a random offset to add to obstacle position */
int rand_offset() {
    /* random number between -128 and 128 */
    return (rand() & 0xff) - 128;
}

/* the four total obstacles are created here */
struct Obstacle obstacles[NUM_OBSTACLES];

/* setup the obstacles */
void obstacles_init() {

    /* randomly position them around */
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].start_x = 300 * (i + 1) + rand_offset() + 200;
    }

    /* setup the sprites */
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        /* half are pits, half are mounds */
        if (i & 1) {
            /* a pit */
            obstacles[i].y = 139;
            obstacles[i].width = 32;
            obstacles[i].sprite = sprite_init(obstacles[i].x, obstacles[i].y,
                    SIZE_32_8, 0, 0, PIT_OFFSET, 1);
        } else {
            /* a mound */
            obstacles[i].y = 135;
            obstacles[i].width = 16;
            obstacles[i].sprite = sprite_init(obstacles[i].x, obstacles[i].y,
                    SIZE_16_8, 0, 0, MOUND_OFFSET, 1);
        }
    }
}

/* update the obstacle position relative to the scrolling */
void obstacles_update(int scroll) {
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        /* update position relative to scrolling */
        obstacles[i].x = obstacles[i].start_x - scroll;

        /* check if off screen */
        if (obstacles[i].x < 0) {
            /* move it down a ways again */
            obstacles[i].start_x = scroll + 1200 + rand_offset();
            obstacles[i].x = obstacles[i].start_x - scroll;
        }

        /* if on screen show it, otherwise hide it */
        if (obstacles[i].x >= 0 && obstacles[i].x < SCREEN_WIDTH) {
            sprite_position(obstacles[i].sprite, obstacles[i].x, obstacles[i].y);
        } else {
            sprite_position(obstacles[i].sprite, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
}

/* check if the rover has crashed into an obstacle or not */
int obstacles_crash(struct Rover* rover) {
    for (int i = 0; i < NUM_OBSTACLES; i++) {


    }

    return 0;
}

#if 0
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
#endif
