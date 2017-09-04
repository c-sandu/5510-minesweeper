#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include "nokia5110.h"
#include "game.h"

#define LCD_WIDTH	84
#define LCD_HEIGHT	48

/* Custom game characters index in the CHARSET */
#define FIELD_HIDDEN	(0x80)
#define FIELD_EMPTY		(0x81)
#define FIELD_BOMB		(0x82)
#define FIELD_FLAG		(0x83)

#define TIME_POS_X		0
#define TIME_POS_Y		0

#define BOMBCOUNT_POS_X 0
#define BOMBCOUNT_POS_Y 37

#define MESSAGE_POS_X	0
#define MESSAGE_POS_Y	6

#define MAX_STR_LEN	8


/**
 * Draws the board area.
 * Because each field is basically a char, old_values will simply be
 * overwritten.
 * @board: the board configuration to be drawn
 */
void draw_board(minefield_t board[GAME_HEIGHT][GAME_WIDTH]);

/**
 * Moves the cursor in the given direction, also clearing the old position.
 * Does nothing if the new position is outside play area.
 * @game: the game instance
 * @dir: direction in which the cursor will be moved
 */
void move_cursor(game_info_t *game, enum direction dir);

/**
 * Draws the given cursor.
 * @cursor: coordinates where the cursor will be drawn
 */
void draw_cursor(cursor_t cursor);

/**
 * Clears the given cursor.
 * @cursor: coordinates where the cursor will be cleared
 */
void clear_cursor(cursor_t cursor);

/**
 * Draws borders for the play area.
 */
void draw_borders(void);

/**
 * Draws the left pane, also called status area.
 * @game: game instance
 */
void draw_status(game_info_t *game);

/**
 * Draws end-game message in the left pane, based on game win/loss
 */
void draw_message(game_info_t *game);

#endif
