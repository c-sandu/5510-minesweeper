#ifndef __GAME_H
#define __GAME_H

#include <stdint.h>


#define FONT_WIDTH		3
#define FONT_HEIGHT		5

/* includes blank space around a character */
#define FIELD_WIDTH		(FONT_WIDTH + 2)
#define FIELD_HEIGHT	(FONT_HEIGHT + 2)

/* playable area */
#define COLS	12
#define ROWS	6
#define BOMB_COUNT	(((COLS + ROWS) / 2) + (ROWS / 2))

/* includes borders with 0 bombs */
#define GAME_WIDTH	(COLS + 2)
#define GAME_HEIGHT	(ROWS + 2)

/* offset for playable area */
#define OFFSET_X	23
#define OFFSET_Y	4



/* Cursor */
typedef struct {
	uint8_t x;
	uint8_t y;
} cursor_t;

enum direction {LEFT, UP, RIGHT, DOWN};

enum field_state { HIDDEN, CLICKED, FLAGGED };

/* Minefield struct */
typedef struct {
	uint8_t isBomb;
	uint8_t bombCount; /* no. of bombs around this field */
	enum field_state state;
} minefield_t;


/* Game states */
enum game_state { READY, ONGOING, WON, LOST };

/* Game info struct, for easier argument passing */
typedef struct {
	cursor_t cursor;
	minefield_t board[GAME_HEIGHT][GAME_WIDTH];
	enum game_state state;
	uint8_t hidden_count;
	uint8_t flagged_count;
	volatile uint16_t seconds_passed;
} game_info_t;


/* Expands all neighboring fields with no bombs around them */
void expand_neighbor_fields(game_info_t *game, uint8_t y, uint8_t x);

/* Toggles flagged status for the given cell */
void toggle_flag(game_info_t *game);

/* Initializes everything about the game */
void game_init(game_info_t *game);

/* Ends the game, setting appropriate states and exposing bombs on a loss */
void game_end(game_info_t *game);

#endif
