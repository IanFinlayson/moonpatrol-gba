/* rover.c
 * functions related to the rover object */

#include "moonpatrol.h"

/* an array storing the ground height at every position
 * not sure if there is a cleaner way to do this :/ */
int ground_height[256] = {
    3,3,
    2,2,2,
    1,1,1,1,1,1,
    2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,
    1,1,1,
    2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,
    2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,
    2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,
    3,3,3,3,3,3,3,3,3,
    2,2,2,2,2,2,
    1,1,1,1,1,1,
    2,2,2,2,2,2,
    3,3,3,3
};

/* setup the rover */
void rover_init(struct Rover* rover) {
    /* initial rover position */
    rover->x = 104;
    rover->y = 125;

    /* setup our sprites */
    rover->body = sprite_init(0, rover->x, rover->y, SIZE_32_16, 0, 0, 0, 1);
    for (int i = 0; i < 3; i++) {
        rover->wheels[i] = sprite_init(1 + i, rover->x + 12*i, rover->y + 10,
                SIZE_8_8, 0, 0, 32, 0);
    }
    rover->side = 0;
    rover->counter = 0;
}

/* move the rover left or right */
void rover_left(struct Rover* rover) {
    rover->x -= ROVER_SPEED;
}
void rover_right(struct Rover* rover) {
    rover->x += ROVER_SPEED;
}

/* flip the rover animation */
void rover_flip(struct Rover* rover) {
    /* check if it's time to flip */
    if (rover->counter < ROVER_WHEEL_FLIP) {
        rover->counter++;
        return;
    }

    int offset;
    if (rover->side) {
        offset = 34;
        rover->side = 0;
    } else {

        offset = 32;
        rover->side = 1;
    }

    for (int i = 0; i < 3; i++) {
        sprite_set_offset(rover->wheels[i], offset);
    }

    /* reset the counter */
    rover->counter = 0;
}

/* update the rover Y position on screen */
void rover_elevate(struct Rover* rover, int scroll) {
    /* find the index into the ground_height array */
    int index = (rover->x + scroll) % 256;

    /* set this as the rover's position */
    rover->y = 125 - ground_height[index];
}

/* flip the wheel animation in the rover and update it's position
 * relative to the scrolling ground */
void rover_update(struct Rover* rover, int scroll) {
    rover_flip(rover);
    rover_elevate(rover, scroll); 

    /* position the sprite */
    sprite_position(rover->body, rover->x, rover->y);
    for (int i = 0; i < 3; i++) {
        sprite_position(rover->wheels[i], rover->x + 12*i, rover->y + 10);
    }
}
