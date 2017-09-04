#include <avr/io.h>

#include "buttons.h"

void buttons_init(void)
{
	/* Set pins as input */
	BUTTONS_DDR &= ~(1 << BTN_UP);
	BUTTONS_DDR &= ~(1 << BTN_DOWN);
	BUTTONS_DDR &= ~(1 << BTN_LEFT);
	BUTTONS_DDR &= ~(1 << BTN_RIGHT);
	BUTTONS_DDR &= ~(1 << BTN_SELECT);
	BUTTONS_DDR &= ~(1 << BTN_FLAG);

	/* Activate pull-up resistors */
	BUTTONS_PORT |= (1 << BTN_UP);
	BUTTONS_PORT |= (1 << BTN_DOWN);
	BUTTONS_PORT |= (1 << BTN_LEFT);
	BUTTONS_PORT |= (1 << BTN_RIGHT);
	BUTTONS_PORT |= (1 << BTN_SELECT);
	BUTTONS_PORT |= (1 << BTN_FLAG);
}