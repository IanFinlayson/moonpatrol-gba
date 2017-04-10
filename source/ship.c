/* ship.c
 * functions related to the enemy ship object */

#include "moonpatrol.h"
#include <stdio.h>
#include <stdlib.h>

#define SHIP_PATH_SIZE 140
#define SHIP_FRAME_SKIP 5
#define SHIP_LOOP_START 20
#define SHIP_HIDDEN_FRAMES 1000
#define SHIP_FIRE_DELAY 75
#define SHIP_BULLET_SPEED 2

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
    sprite_position(ship->sprite, ship->x, ship->y);

    /* set to hidden */
    ship->visible = 0;
    ship->frames_hidden = 0;

    /* setup the bullets */
    ship->bullet_index = 0;
    for (int i = 0; i < SHIP_NUM_BULLTETS; i++) {
        ship->bullets[i].bullet_sprite = sprite_init(0, 0, SIZE_8_8, 0, 0, 48, 0);
        ship->bullets[i].x = 0;
        ship->bullets[i].y = -100;
        sprite_position(ship->bullets[i].bullet_sprite, ship->bullets[i].x, ship->bullets[i].y);
        ship->bullets[i].alive = 0;
    }
}

/* have the ship fire a bullet */
void ship_fire(struct Ship* ship) {
    /* index the bullet and find next one */
    struct Bullet* bullet = &(ship->bullets[ship->bullet_index]);
    ship->bullet_index++;
    if (ship->bullet_index >= SHIP_NUM_BULLTETS) {
        ship->bullet_index = 0;
    }

    /* place it under the ship */
    bullet->y = ship->y + 12;

    /* randomly fire from the left or right turret */
    if (rand() & 1) {
        bullet->x = ship->x - 3;
    } else {
        bullet->x = ship->x + 18;
    }
    bullet->alive = 1;
}

/* flip the wheel animation in the ship and update it's position
 * relative to the scrolling ground */
int ship_update(struct Ship* ship, int scroll, struct Rover* rover) {
    if (!ship->visible) {
        /* update hidden frames */
        ship->frames_hidden++;

        /* check if time to show up */
        if (ship->frames_hidden >= SHIP_HIDDEN_FRAMES) {
            ship->visible = 1;
            ship->fire_countdown = SHIP_FIRE_DELAY;
        } else {
            return 0;
        }
    }

    /* check if this is a skip frame */
    ship->update_frames++;
    if (ship->update_frames >= SHIP_FRAME_SKIP) {
        ship->update_frames = 0;
    }
    if (ship->update_frames > 0) {
        return 0;
    }

    /* check if it's time to fire */
    ship->fire_countdown--;
    if (ship->fire_countdown == 0) {
        /* fire the bullet */
        ship_fire(ship);

        /* reset the counter */
        ship->fire_countdown = SHIP_FIRE_DELAY;
    }

    /* update the bullets */
    for (int i = 0; i < SHIP_NUM_BULLTETS; i++) {
        if (ship->bullets[i].alive) {
            ship->bullets[i].y += SHIP_BULLET_SPEED;
            sprite_position(ship->bullets[i].bullet_sprite, ship->bullets[i].x, ship->bullets[i].y); 

            /* check if it hit the rover */
            if ((ship->bullets[i].y - 4) >= (rover->y >> VERT_SHIFT_AMOUNT)) {
                if (ship->bullets[i].x > (rover->x + 32)) {
                    /* no collision */
                }
                else if ((ship->bullets[i].x + 8) < rover->x) {
                    /* no collision */
                } else {
                    /* BOOM */
                    return 1;
                }
            }

            /* check if the bullet hit the ground */
            if (ship->bullets[i].y > 140) {
                ship->bullets[i].x = 0;
                ship->bullets[i].y = -100;
                sprite_position(ship->bullets[i].bullet_sprite, ship->bullets[i].x, ship->bullets[i].y);
                ship->bullets[i].alive = 0;
            }
        }
    }


    /* update the ship position based on the paths */
    ship->x += ship_x_directions[ship->movement_counter];
    ship->y += ship_y_directions[ship->movement_counter];

    /* update the counter */
    ship->movement_counter++;
    if (ship->movement_counter >= SHIP_PATH_SIZE) {
        ship->movement_counter = SHIP_LOOP_START;
    }

    /* position the sprite */
    sprite_position(ship->sprite, ship->x, ship->y);
    return 0;
}

