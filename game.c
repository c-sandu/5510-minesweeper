#include <stdlib.h>
#include <avr/io.h>

#include "game.h"
#include "buttons.h"

/**
 * Clears every field on the board
 * @board: a fixed size board to be filled with initial values
 */
static void board_init(minefield_t board[GAME_HEIGHT][GAME_WIDTH])
{
	register uint8_t i, j;
	for (i = 0; i < GAME_HEIGHT; ++i)
		for (j = 0; j < GAME_WIDTH; ++j) {
			board[i][j].isBomb = 0;
			board[i][j].bombCount = 0;
			board[i][j].state = HIDDEN;
		}

	/* also make sure the borders are revealed */
	for (i = 0; i < GAME_HEIGHT; ++i) {
		board[i][0].state = CLICKED;
		board[i][GAME_WIDTH - 1].state = CLICKED;
	}

	for (i = 0; i < GAME_WIDTH; ++i) {
		board[0][i].state = CLICKED;
		board[GAME_HEIGHT - 1][i].state = CLICKED;
	}
}

/**
 * Randomly places bombs in the given board, while also
 * @board: board to be filled with bombs
 */
static void generate_bombs(minefield_t board[GAME_HEIGHT][GAME_WIDTH])
{
	uint8_t count = 0;
	uint8_t bomb_x, bomb_y;

	while (count < BOMB_COUNT) {
		bomb_x = 1 + rand() % COLS;
		bomb_y = 1 + rand() % ROWS;

		if (!board[bomb_y][bomb_x].isBomb) {
			board[bomb_y][bomb_x].isBomb = 1;

			count++;
		}
	}
}

/**
 * Computes and fills in bomb count for each field.
 * @board: board for which hints will be computed
 */
static void fill_in_bomb_count(minefield_t board[GAME_HEIGHT][GAME_WIDTH])
{
	register uint8_t x, y;

	for (y = 1; y <= ROWS; y++)
		for (x = 1; x <= COLS; x++)
			if (!board[y][x].isBomb) {
				board[y][x].bombCount += board[y-1][x-1].isBomb ? 1 : 0;
				board[y][x].bombCount += board[y-1][ x ].isBomb ? 1 : 0;
				board[y][x].bombCount += board[y-1][x+1].isBomb ? 1 : 0;
				board[y][x].bombCount += board[ y ][x-1].isBomb ? 1 : 0;
				board[y][x].bombCount += board[ y ][x+1].isBomb ? 1 : 0;
				board[y][x].bombCount += board[y+1][x-1].isBomb ? 1 : 0;
				board[y][x].bombCount += board[y+1][ x ].isBomb ? 1 : 0;
				board[y][x].bombCount += board[y+1][x+1].isBomb ? 1 : 0;
			}
}

void expand_neighbor_fields(game_info_t *game, uint8_t y, uint8_t x)
{
	if (y > 0 && x > 0 && y < GAME_HEIGHT && x < GAME_WIDTH)
	{
		if (game->board[y][x].state == HIDDEN) {
			if (!game->board[y][x].isBomb) {
				game->board[y][x].state = CLICKED;
				game->hidden_count--;

				if (game->board[y][x].bombCount == 0) {
					expand_neighbor_fields(game,y-1,x-1);
					expand_neighbor_fields(game,y-1, x );
					expand_neighbor_fields(game,y-1,x+1);
					expand_neighbor_fields(game, y ,x-1);
					expand_neighbor_fields(game, y ,x+1);
					expand_neighbor_fields(game,y+1,x-1);
					expand_neighbor_fields(game,y+1, x );
					expand_neighbor_fields(game,y+1,x+1);
				}
			}
		}
	}
}

/**
 * Reveals all remaining bombs, called at the end of the game.
 * @board: board to be revealed
 */
static void expose_all_mines(minefield_t board[GAME_HEIGHT][GAME_WIDTH])
{
	register uint8_t x, y;

	for (y = 1; y <= ROWS; ++y)
		for (x = 1; x <= COLS; ++x)
			if (board[y][x].isBomb)
				board[y][x].state = CLICKED;
}

void toggle_flag(game_info_t *game)
{
	if (game->board[game->cursor.y][game->cursor.x].state == HIDDEN) {
		if (game->flagged_count < BOMB_COUNT) {
			game->board[game->cursor.y][game->cursor.x].state = FLAGGED;
			game->flagged_count++;
		}
	}
	else if (game->board[game->cursor.y][game->cursor.x].state == FLAGGED) {
		game->board[game->cursor.y][game->cursor.x].state = HIDDEN;
		game->flagged_count--;
	}
}

void game_init(game_info_t *game)
{
	board_init(game->board);
	generate_bombs(game->board);
	fill_in_bomb_count(game->board);

	game->cursor.y = 1;
	game->cursor.x = 1;

	game->hidden_count = ROWS * COLS;
	game->flagged_count = 0;

	game->seconds_passed = 0;

	game->state = READY;
}

void game_end(game_info_t *game)
{
	if (game->board[game->cursor.y][game->cursor.x].isBomb) {
		game->state = LOST;
		expose_all_mines(game->board);
	} else {
		game->state = WON;
	}
}
