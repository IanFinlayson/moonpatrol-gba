/* audio.c
 * functions to handle audio playback */

#include "gba.h"

extern const signed char music_16K_mono [];

/* global variables to keep track of how much longer the sounds are to play */
unsigned int channel_a_vblanks_remaining = 0;
unsigned int channel_a_total_vblanks = 0;
unsigned int channel_b_vblanks_remaining = 0;


/* play a sound with a number of samples, and sample rate on one channel 'A' or 'B' */
void play_sound(const signed char* sound, int total_samples, int sample_rate, char channel) {
    /* start by disabling the timer and dma controller (to reset a previous sound) */
    TIMER0_CONTROL = 0;
    if (channel == 'A') {
        REG_DMA1_CONTROL = 0;
    } else if (channel == 'B') {
        REG_DMA2_CONTROL = 0; 
    }

    /* output to both sides and reset the FIFO */
    if (channel == 'A') {
        SOUND_CONTROL |= SOUND_A_RIGHT_CHANNEL | SOUND_A_LEFT_CHANNEL | SOUND_A_FIFO_RESET;
    } else if (channel == 'B') {
        SOUND_CONTROL |= SOUND_B_RIGHT_CHANNEL | SOUND_B_LEFT_CHANNEL | SOUND_B_FIFO_RESET;
    }

    /* enable all sound */
    MASTER_SOUND = SOUND_MASTER_ENABLE;

    /* set the dma channel to transfer from the sound array to the sound buffer */
    if (channel == 'A') {
        REG_DMA1_SOURCE = (unsigned int) sound;
        REG_DMA1_DESTINATION = (unsigned int) FIFO_BUFFER_A;
        REG_DMA1_CONTROL = DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
    } else if (channel == 'B') {
        REG_DMA2_SOURCE = (unsigned int) sound;
        REG_DMA2_DESTINATION = (unsigned int) FIFO_BUFFER_B;
        REG_DMA2_CONTROL = DMA_DEST_FIXED | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
    }

    /* set the timer so that it increments once each time a sample is due
     * we divide the clock (ticks/second) by the sample rate (samples/second)
     * to get the number of ticks/samples */
    unsigned short ticks_per_sample = CLOCK / sample_rate;

    /* the timers all count up to 65536 and overflow at that point, so we count up to that
     * now the timer will trigger each time we need a sample, and cause DMA to give it one! */
    TIMER0_DATA = 65536 - ticks_per_sample;

    /* determine length of playback in vblanks
     * this is the total number of samples, times the number of clock ticks per sample,
     * divided by the number of machine cycles per vblank (a constant) */
    if (channel == 'A') {
        channel_a_vblanks_remaining = total_samples * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
        channel_a_total_vblanks = channel_a_vblanks_remaining;
    } else if (channel == 'B') {
        channel_b_vblanks_remaining = total_samples * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
    }

    /* enable the timer */
    TIMER0_CONTROL = TIMER_ENABLE | TIMER_FREQ_1;
}


/* this function is called each vblank to get the timing of sounds right */
void on_vblank() {
    /* disable interrupts for now and save current state of interrupt */
    INTERRUPT_ENABLE = 0;
    unsigned short temp = INTERRUPT_STATE;

    /* look for vertical refresh */
    if ((INTERRUPT_STATE & INTERRUPT_VBLANK) == INTERRUPT_VBLANK) {

        /* update channel A */
        if (channel_a_vblanks_remaining == 0) {
            /* restart the sound again when it runs out */
            channel_a_vblanks_remaining = channel_a_total_vblanks;
            REG_DMA1_CONTROL = 0;
            REG_DMA1_SOURCE = (unsigned int) music_16K_mono;
            REG_DMA1_CONTROL = DMA_DEST_FIXED | DMA_REPEAT | DMA_32 |
                DMA_SYNC_TO_TIMER | DMA_ENABLE;
        } else {
            channel_a_vblanks_remaining--;
        }

        /* update channel B */
        if (channel_b_vblanks_remaining == 0) {
            /* disable the sound and DMA transfer on channel B */
            SOUND_CONTROL &= ~(SOUND_B_RIGHT_CHANNEL | SOUND_B_LEFT_CHANNEL | SOUND_B_FIFO_RESET);
            REG_DMA2_CONTROL = 0;
        }
        else {
            channel_b_vblanks_remaining--;
        }
    }

    /* restore/enable interrupts */
    INTERRUPT_STATE = temp;
    INTERRUPT_ENABLE = 1;
}


