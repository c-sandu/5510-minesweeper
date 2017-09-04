/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "nokia5110.h"
#include "buttons.h"
#include "game.h"
#include "graphics.h"
#include "utils.h"



game_info_t game;

/**
 * Interrupt Service Routine for the game clock.
 */
ISR(TIMER1_COMPA_vect)
{
	if (game.state == ONGOING) {
		game.seconds_passed++;
		draw_status(&game);
	}
	nokia_lcd_render();
}


int main(void)
{
	/* global interrupt enable */
	sei();

	/* init the timer, with 1Hz freq */
	rand_seed_init();
	timer_init();

	/* init the user buttons */
	buttons_init();

	/* init the display */
	nokia_lcd_init();
	nokia_lcd_clear();

	/* init the game with starting values */
	game_init(&game);

	/* draw elements */
	draw_board(game.board);
	draw_borders();
	draw_cursor(game.cursor);
	draw_status(&game);

	nokia_lcd_render();

	for (;;) {
		if ((BUTTONS_PIN & (1 << BTN_LEFT)) == 0) {
			if (game.state == READY) {
				game.state = ONGOING;
			} else if (game.state == ONGOING) {
				move_cursor(&game, LEFT);
			} else if (game.state == WON || game.state == LOST) {
				/* do nothing */
			}
			_delay_ms(BTN_PRESS_DELAY_MS);
		}
		else if ((BUTTONS_PIN & (1 << BTN_UP)) == 0) {
			if (game.state == READY) {
				game.state = ONGOING;
			} else if (game.state == ONGOING) {
				move_cursor(&game, UP);
			} else if (game.state == WON || game.state == LOST) {
				/* do nothing */
			}
			_delay_ms(BTN_PRESS_DELAY_MS);
		}
		else if ((BUTTONS_PIN & (1 << BTN_RIGHT)) == 0) {
			if (game.state == READY) {
				game.state = ONGOING;
			} else if (game.state == ONGOING) {
				move_cursor(&game, RIGHT);
			} else if (game.state == WON || game.state == LOST) {
				/* do nothing */
			}
			_delay_ms(BTN_PRESS_DELAY_MS);
		}
		else if ((BUTTONS_PIN & (1 << BTN_DOWN)) == 0) {
			if (game.state == READY) {
				game.state = ONGOING;
			} else if (game.state == ONGOING) {
				move_cursor(&game, DOWN);
			} else if (game.state == WON || game.state == LOST) {
				/* do nothing */
			}
			_delay_ms(BTN_PRESS_DELAY_MS);
		}
		else if ((BUTTONS_PIN & (1 << BTN_SELECT)) == 0) {
			if (game.state == READY) {
				game.state = ONGOING;
			} else if (game.state == ONGOING) {
				if (game.board[game.cursor.y][game.cursor.x].isBomb &&
					 game.board[game.cursor.y][game.cursor.x].state != FLAGGED) {

					game_end(&game);
					draw_message(&game);
				}

				/* expand field and all neighbors surrounded by 0 bombs */
				expand_neighbor_fields(&game, game.cursor.y, game.cursor.x);

				/* win condition */
				if (game.hidden_count == BOMB_COUNT) {
					game_end(&game);
					draw_message(&game);
				}

				/* board update, so we need to redraw it */
				draw_board(game.board);
				nokia_lcd_render();
			} else if (game.state == WON || game.state == LOST) {
				/* do nothing */
			}
			_delay_ms(BTN_PRESS_DELAY_MS);
		}
		else if ((BUTTONS_PIN & (1 << BTN_FLAG)) == 0) {
			if (game.state == READY) {
				game.state = ONGOING;
			} else if (game.state == ONGOING) {
				toggle_flag(&game);

				/* board and status update */
				draw_board(game.board);
				draw_status(&game);
				nokia_lcd_render();
			} else if (game.state == WON || game.state == LOST) {
				game_init(&game);

				/* reset the game */
				nokia_lcd_clear();
				draw_board(game.board);
				draw_borders();
				draw_cursor(game.cursor);
				draw_status(&game);
				nokia_lcd_render();
			}

			_delay_ms(BTN_PRESS_DELAY_MS);
		}
	}
}
