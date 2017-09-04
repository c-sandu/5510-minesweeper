#include "graphics.h"
#include "nokia5110.h"
#include "game.h"

#include <stdio.h>

void draw_board(minefield_t board[GAME_HEIGHT][GAME_WIDTH])
{
	register uint8_t x, y;

	for (y = 1; y <= ROWS; ++y)
		for (x = 1; x <= COLS; ++x) {
			nokia_lcd_set_cursor(OFFSET_X + FIELD_WIDTH * (x - 1),
								OFFSET_Y + FIELD_HEIGHT * (y - 1));

			if (board[y][x].state == HIDDEN)
				nokia_lcd_write_char(FIELD_HIDDEN);
			else if (board[y][x].state == CLICKED) {
				if (board[y][x].isBomb)
					nokia_lcd_write_char(FIELD_BOMB);
				else if (board[y][x].bombCount == 0)
					nokia_lcd_write_char(FIELD_EMPTY);
				else
					nokia_lcd_write_char('0' + board[y][x].bombCount);
			} else if (board[y][x].state == FLAGGED)
				nokia_lcd_write_char(FIELD_FLAG);
		}
}

void draw_cursor(cursor_t cursor)
{
	nokia_lcd_draw_rect(
		OFFSET_X + FIELD_WIDTH * (cursor.x - 1) - 2,
		OFFSET_Y + FIELD_HEIGHT * (cursor.y - 1) - 2,
		FIELD_WIDTH + 2,
		FIELD_HEIGHT + 2,
		0x01,
		RECT_NO_FILL);
}

void clear_cursor(cursor_t old_cursor)
{
	nokia_lcd_draw_rect(
		OFFSET_X + FIELD_WIDTH * (old_cursor.x - 1) - 2,
		OFFSET_Y + FIELD_HEIGHT * (old_cursor.y - 1) - 2,
		FIELD_WIDTH + 2,
		FIELD_HEIGHT + 2,
		0x00,
		RECT_NO_FILL);
}

void draw_borders(void)
{
	nokia_lcd_draw_rect(OFFSET_X - 3, OFFSET_Y - 3,
		FIELD_WIDTH * COLS + 4, FIELD_HEIGHT * ROWS + 4,
		0x01, RECT_NO_FILL);
}

/**
 * Draws the time in the status area.
 * @game: the game instance whose seconds_passed field will be displayed
 */
static void draw_time(game_info_t *game) {
	char tmp[MAX_STR_LEN];

	sprintf(tmp, "t:%d", game->seconds_passed);
	nokia_lcd_set_cursor(TIME_POS_X, TIME_POS_Y);
	nokia_lcd_write_string(tmp);
}

/**
 * Draws the bombcount in the status area.
 * Format will be: "bombs_remaining/total_bombs"
 * @game: the game instance whose info is to be displayed
 */
static void draw_bombcount(game_info_t *game) {
	char tmp[MAX_STR_LEN];

	nokia_lcd_set_cursor(BOMBCOUNT_POS_X, BOMBCOUNT_POS_Y);
	nokia_lcd_write_string("bombs");

	nokia_lcd_set_cursor(BOMBCOUNT_POS_X, BOMBCOUNT_POS_Y + FONT_HEIGHT + 1);
	sprintf(tmp, "%d/%d", BOMB_COUNT - game->flagged_count, BOMB_COUNT);
	nokia_lcd_write_string(tmp);
}

void draw_status(game_info_t *game) {

	// clear status area
	nokia_lcd_draw_rect(0, 0, OFFSET_X - 3, LCD_HEIGHT, 0x00, RECT_FILL);

	draw_time(game);

	draw_bombcount(game);
}

void draw_message(game_info_t *game) {

	nokia_lcd_set_cursor(MESSAGE_POS_X + 3, MESSAGE_POS_Y);
	nokia_lcd_write_string("YOU");
	nokia_lcd_set_cursor(MESSAGE_POS_X, MESSAGE_POS_Y + FONT_HEIGHT + 1);
	if (game->state == WON)
		nokia_lcd_write_string("WON!");
	else
		nokia_lcd_write_string("LOST!");

	nokia_lcd_set_cursor(MESSAGE_POS_X,
		MESSAGE_POS_Y + 3 * (FONT_HEIGHT + 1));
	nokia_lcd_write_string("Press");

	nokia_lcd_set_cursor(MESSAGE_POS_X + 2,
		MESSAGE_POS_Y + 4 * (FONT_HEIGHT + 1));
	nokia_lcd_write_string("FLAG");
}

void move_cursor(game_info_t *game, enum direction dir)
{
	if (dir == LEFT && game->cursor.x > 1) {
		clear_cursor(game->cursor);
		game->cursor.x--;
		draw_cursor(game->cursor);
	} else if (dir == UP && game->cursor.y > 1) {
		clear_cursor(game->cursor);
		game->cursor.y--;
		draw_cursor(game->cursor);
	} else if (dir == RIGHT && game->cursor.x < COLS) {
		clear_cursor(game->cursor);
		game->cursor.x++;
		draw_cursor(game->cursor);
	} else if (dir == DOWN && game->cursor.y < ROWS) {
		clear_cursor(game->cursor);
		game->cursor.y++;
		draw_cursor(game->cursor);
	}

	nokia_lcd_render();
}