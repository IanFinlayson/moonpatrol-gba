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

    sprite_position(rover->body, rover->x, rover->y);
    for (int i = 0; i < 3; i++) {
        sprite_position(rover->wheels[i], rover->x + 12*i, rover->y + 10);
    }
}
void rover_right(struct Rover* rover) {
    rover->x += ROVER_SPEED;

    sprite_position(rover->body, rover->x, rover->y);
    for (int i = 0; i < 3; i++) {
        sprite_position(rover->wheels[i], rover->x + 12*i, rover->y + 10);
    }
}

/* flip the wheel animation in the rover and update it's position
 * relative to the scrolling ground */
void rover_update(struct Rover* rover, int scorll) {
    int offset;

    /* check if it's time to flip */
    if (rover->counter < ROVER_WHEEL_FLIP) {
        rover->counter++;
        return;
    }

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
