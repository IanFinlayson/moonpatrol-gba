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

/* perform fast DMA memory copies */
void dma_memcpy(void* source, void* dest, unsigned count, unsigned mode);

/* write text to a specific tile on the screen */
void set_text(char* str, int row, int col, unsigned short* textmap);


#endif

