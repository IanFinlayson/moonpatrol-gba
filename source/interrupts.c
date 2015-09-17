/*
 * interrupts.c
 * this file contains the intterupt functions for the game, alond with the
 * intterupt table needed to link GBA executables
 */

/* the interrupt functions are this type */
typedef void (*intrp)( );

/* this function is used for interrupts which we ignore */
void interrupt_ignore( ) {
}

/* this table specifies which interrupts we handle which way */
const intrp IntrTable[13] = {
    interrupt_ignore,   /* V Blank interrupt */
    interrupt_ignore,   /* H Blank interrupt */
    interrupt_ignore,   /* V Counter interrupt */
    interrupt_ignore,   /* Timer 0 interrupt */
    interrupt_ignore,   /* Timer 1 interrupt */
    interrupt_ignore,   /* Timer 2 interrupt */
    interrupt_ignore,   /* Timer 3 interrupt */
    interrupt_ignore,   /* Serial communication interrupt */
    interrupt_ignore,   /* DMA 0 interrupt */
    interrupt_ignore,   /* DMA 1 interrupt */
    interrupt_ignore,   /* DMA 2 interrupt */
    interrupt_ignore,   /* DMA 3 interrupt */
    interrupt_ignore,   /* Key interrupt */
};

