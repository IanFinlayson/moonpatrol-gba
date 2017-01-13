/* ship.c
 * functions related to the enemy ship object */

#include "moonpatrol.h"
#include <stdio.h>

#define SHIP_PATH_SIZE 140
#define SHIP_FRAME_SKIP 5
#define SHIP_LOOP_START 20

/* the list of directions that the ship takes in the x and y directions */
int ship_x_directions [SHIP_PATH_SIZE] = {
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1,  2,  2,  2,  2,  3,  3,  3, 
     3,  3,  3,  2,  2,  2,  2,  1,  1,  1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 -1, -1, -1, -2, -2, -2, -2, -3, -3, -3,
    -3, -3, -3, -2, -2, -2, -2, -1, -1, -1 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  1, 
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

int ship_y_directions [SHIP_PATH_SIZE] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 
    0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 1, 1, 1, 1, 
    2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0, -1, -1, -1, -2, -2, -2, -3, -3,
    -3, -3, -2, -2, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 
	-1, -2, -2, -2, -3, -3, -3, -2, -2, -2, -1, -1, -1, 0, 0, 0, 1, 1, 2, 2
};


/* setup the ship */
void ship_init(struct Ship* ship) {
    /* the ship starts at the beginning of the movement path */
    ship->movement_counter = 0;
    ship->update_frames = 0;

    /* setup our sprites */
    ship->sprite = sprite_init(0, 0, SIZE_32_16, 0, 0, 16, 0);

    /* initial ship position */
    ship->x = 55;
    ship->y = -25;
}

/* flip the wheel animation in the ship and update it's position
 * relative to the scrolling ground */
void ship_update(struct Ship* ship, int scroll) {
    /* check if this is a skip frame */
    ship->update_frames++;
    if (ship->update_frames >= SHIP_FRAME_SKIP) {
        ship->update_frames = 0;
    }
    if (ship->update_frames > 0) {
        return;
    }

    /* update the ship position based on the paths */
    ship->x += ship_x_directions[ship->movement_counter];
    ship->y += ship_y_directions[ship->movement_counter];

    /* update the counter */
    ship->movement_counter++;
    if (ship->movement_counter >= SHIP_PATH_SIZE) {
        ship->movement_counter = SHIP_LOOP_START;
    }

    char location[16];
    sprintf(location, "(%d, %d)", ship->x, ship->y);
    set_text(location, 0, 0);

    /* position the sprite */
    sprite_position(ship->sprite, ship->x, ship->y);
}

