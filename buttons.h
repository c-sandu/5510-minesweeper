#ifndef __BUTTONS_H
#define __BUTTONS_H

/* Buttons PORT and DDR */
#define BUTTONS_PORT	PORTA
#define BUTTONS_DDR		DDRA
#define BUTTONS_PIN		PINA

/* Button pins */
#define BTN_UP		PA2
#define BTN_DOWN	PA3
#define BTN_LEFT	PA4
#define BTN_RIGHT	PA1
#define BTN_SELECT	PA5
#define BTN_FLAG	PA0

/* button press delay */
#define BTN_PRESS_DELAY_MS	250

/**
 * Init function for buttons
 * Activates pull-up resistors and sets the corresponding pins as input
 */
void buttons_init(void);

#endif
