#ifndef __UTILS_H
#define __UTILS_H

/**
 * Uses the ADC on floating pin to get some entropy and seed the
 * random number generator
 */
void rand_seed_init();


/**
 * Initializes 1s timer for the game clock.
 * Uses timer1 with CTC mode, top at OCR1A(0x3D09) and prescaler value of 1024
 */
void timer_init();

#endif
