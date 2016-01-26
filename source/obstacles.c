/* obstacles.c
 * functions related to the two types of obstacles: pits and mounds */

#include "moonpatrol.h"

/* how many obstacles there are */
#define NUM_OBSTACLES 4

#define PIT_OFFSET 36
#define MOUND_OFFSET 44

/* the four total obstacles are created here */
struct Obstacle obstacles[NUM_OBSTACLES];

/* setup the obstacles */
void obstacles_init() {
    
    /* TODO randomly position them around */
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].start_x = 100 << i;
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
        obstacles[i].x = obstacles[i].start_x - scroll;
        sprite_position(obstacles[i].sprite, obstacles[i].x, obstacles[i].y);
    }
}




