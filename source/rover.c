/* rover.c
 * functions related to the rover object */

#include "moonpatrol.h"

/* the number of wheels on the rover */
#define NUM_WHEELS 3

/* the size of each wheel in pixels square */
#define WHEEL_SIZE 8

/* the pixels between each wheel when drawn */
#define WHEEL_SPACING 12

/* the speed of the rover is one pixel per this many frames */
#define ROVER_INVERSE_SPEED 2

/* the x and y boundaries for the left side of the rover */
#define GAP 70
#define ROVER_LEFT_BOUND GAP
#define ROVER_RIGHT_BOUND ((240 - GAP) - 32)

/* frames in between rover wheel flips */
#define ROVER_WHEEL_FLIP 15

/* the pixels down the wheels are from the top of the rover */
#define WHEEL_DROP 9

/* the base height of the ground */
#define GROUND_HEIGHT 125

/* the tile offset in our objects image that the wheel frames are */
#define WHEEL_FRAME_1 32
#define WHEEL_FRAME_2 34

/* the units here are 1/256 pixels
 * jump speed is the pixels/256 our y speed is set to on jumping
 * gravity is the pixels/256/second/second we lose on speed each frame */
#define JUMP_SPEED 130
#define GRAVITY 2

/* the shift amount to convert pixels to/from pixels/256 */
#define VERT_SHIFT_AMOUNT 8

/* an array storing the ground height at every position, this is relative
 * to the lowest height of the ground.  this is used in calculating what
 * height each wheel, and the rover itself should be at */
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
    rover->y = GROUND_HEIGHT;

    /* setup our sprites */
    rover->body = sprite_init(rover->x, rover->y, SIZE_32_16, 0, 0, 0, 1);
    for (int i = 0; i < NUM_WHEELS; i++) {
        rover->wheels[i] = sprite_init(rover->x + WHEEL_SPACING * i,
                rover->y + 10, SIZE_8_8, 0, 0, WHEEL_FRAME_1, 0);
    }

    /* start at the first animation frame for the wheels */
    rover->side = 0;
    rover->anim_counter = 0;
    rover->move_counter = 0;

    /* we are not jumping to start */
    rover->dy = 0;
    rover->jumping = 0;
}

/* move the rover left or right */
void rover_left(struct Rover* rover) {
    if (rover->x >  ROVER_LEFT_BOUND) {
        if (rover->move_counter == ROVER_INVERSE_SPEED) {
            rover->move_counter = 0;
            rover->x--;
        } else {
            rover->move_counter++;
        }
    }
}
void rover_right(struct Rover* rover) {
    if (rover->x < ROVER_RIGHT_BOUND) {
        if (rover->move_counter == ROVER_INVERSE_SPEED) {
            rover->move_counter = 0;
            rover->x++;
        } else {
            rover->move_counter++;
        }
    }
}

/* flip the rover animation */
void rover_flip(struct Rover* rover) {
    /* check if it's not time to flip yet */
    if (rover->anim_counter < ROVER_WHEEL_FLIP) {
        rover->anim_counter++;
        return;
    }

    /* pick which frame we should display */
    int offset;
    if (rover->side) {
        offset = WHEEL_FRAME_2;
        rover->side = 0;
    } else {

        offset = WHEEL_FRAME_1;
        rover->side = 1;
    }

    /* update the frame by changing the tile
     * offset in the sprite for each wheel */
    for (int i = 0; i < NUM_WHEELS; i++) {
        sprite_set_offset(rover->wheels[i], offset);
    }

    /* reset the counter */
    rover->anim_counter = 0;
}

/* update the rover Y position on screen */
void rover_elevate(struct Rover* rover, int scroll) {
    /* find the height of each wheel
     * & 0xff is the same thing as % 256 but faster */
    for (int i = 0; i < NUM_WHEELS; i++) {
        rover->wheel_height[i] = (GROUND_HEIGHT - ground_height[(rover->x +
                scroll + (WHEEL_SIZE >> 1) + WHEEL_SPACING * i) & 0xff])
                << VERT_SHIFT_AMOUNT;
    }

    /* set the rover's position to that of the highest wheel */
    rover->y = rover->wheel_height[0];
    for (int i = 1; i < NUM_WHEELS; i++) {
        if (rover->y > rover->wheel_height[i]) {
            rover->y = rover->wheel_height[i];;
        }
    }
}

/* flip the wheel animation in the rover and update it's position
 * relative to the scrolling ground */
void rover_update(struct Rover* rover, int scroll) {
    /* flip the animation, and calculate the rover elevation */
    rover_flip(rover);

    /* update position based on jumping */
    if (rover->jumping) {
        /* adjust for jumping */
        rover->y += rover->dy;

        /* set wheels to the same height (shifted down a scoche) */
        for (int i = 0; i < NUM_WHEELS; i++) {
            rover->wheel_height[i] = rover->y + 512;
        }

        /* decelerate */
        rover->dy += GRAVITY;

    } else {
        /* else position based on the ground */
        rover_elevate(rover, scroll); 
    }

    /* if we are at ground level again, stop jumping */
    if (rover->jumping && (rover->y >> VERT_SHIFT_AMOUNT) > GROUND_HEIGHT) {
        rover->jumping = 0;
    }

    /* position the sprite */
    sprite_position(rover->body, rover->x, rover->y >> VERT_SHIFT_AMOUNT);
    for (int i = 0; i < NUM_WHEELS; i++) {
        sprite_position(rover->wheels[i], rover->x + WHEEL_SPACING * i,
                (rover->wheel_height[i] >> VERT_SHIFT_AMOUNT) + WHEEL_DROP);
    }
}

/* jump the rover into the air */
void rover_jump(struct Rover* rover) {
    /* no double jumps allowed */
    if (!rover->jumping) {
        /* begin moving up! */
        rover->dy = -JUMP_SPEED;
        rover->jumping = 1;
    }
}

/* crash the rover when it has been destroyed */
void rover_crash(struct Rover* rover) {
    /* move down a bit */
    rover->y += (3 << VERT_SHIFT_AMOUNT);

    /* keep track of the wheel postions */
    int wheelx[3], wheely[3];
    for (int i = 0; i < 3; i++) {
        wheelx[i] = rover->x + WHEEL_SPACING * i;
        wheely[i] = (rover->wheel_height[i] >> VERT_SHIFT_AMOUNT) + WHEEL_DROP;
    }

    for (int i = 0; i < 1000; i++) {
        /* move the wheels some */
        wheelx[0]--;
        wheelx[2]++;
        wheely[0]++;
        wheely[1]++;
        wheely[2]++;

        /* set the position */
        sprite_position(rover->body, rover->x, rover->y >> VERT_SHIFT_AMOUNT);
        for (int i = 0; i < NUM_WHEELS; i++) {
            sprite_position(rover->wheels[i], wheelx[i], wheely[i]);
        }

        /* wait for vertical refresh again */
        wait_vblank();

        /* update all sprites on screen */
        sprite_update_all();

        /* delay for a little bit */
        delay(500);
    }
}

