/* rover.c
 * functions related to the rover object */

#include "moonpatrol.h"

/* setup the rover */
void rover_init(struct Rover* rover) {
    /* setup our sprites */
    rover->body = setup_sprite(0, 104, 125, SIZE_32_16, 0, 0, 0, 1);
    int i;
    for (i = 0; i < 3; i++) {
        rover->wheels[i] = setup_sprite(1 + i, 104 + 12*i, 135, SIZE_8_8, 0, 0, 32, 0);
    }
    rover->side = 0;
    rover->counter = 0;
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

    for (i = 0; i < 3; i++) {
        sprite_set_offset(rover->wheels[i], offset);
    }

    /* reset the counter */
    rover->counter = 0;
}
