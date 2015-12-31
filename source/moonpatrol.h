/*
 * moonpatrol.h
 * this file contains function and type declarations
 */

#ifndef MOONPATROL_H
#define MOONPATROL_H

/* the width and height of the screen */
#define WIDTH 240
#define HEIGHT 160

/* wait for a vertical refresh */
void wait_vblank();

/* delay for an amount of time */
void delay(int amount);

/* check if a button is down */
int button_down(int button);

#endif

