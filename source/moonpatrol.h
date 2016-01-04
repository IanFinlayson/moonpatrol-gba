/*
 * moonpatrol.h
 * this file contains function and type declarations
 */

#ifndef MOONPATROL_H
#define MOONPATROL_H

/* wait for a vertical refresh */
void wait_vblank(void);

/* delay for an amount of time */
void delay(int amount);

/* check if a button is down */
int button_down(int button);

/* initialize a background */
void init_background(int bg, int priority, int screenblock);


#endif

