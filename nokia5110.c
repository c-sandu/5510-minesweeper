/* Nokia 5110 LCD AVR Library
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

#include "nokia5110.h"

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110_chars.h"


static struct {
	/* screen byte massive */
	uint8_t screen[504];

	/* cursor position */
	uint8_t cursor_x;
	uint8_t cursor_y;

} nokia_lcd = {
	.cursor_x = 0,
	.cursor_y = 0
};

/**
 * Sending data to LCD
 * @bytes: data
 * @is_data: transfer mode: 1 - data; 0 - command;
 */
static void write(uint8_t bytes, uint8_t is_data)
{
	register uint8_t i;
	/* Enable controller */
	PORT_LCD &= ~(1 << LCD_SCE);

	/* We are sending data */
	if (is_data)
		PORT_LCD |= (1 << LCD_DC);
	/* We are sending commands */
	else
		PORT_LCD &= ~(1 << LCD_DC);

	/* Send bytes */
	for (i = 0; i < 8; i++) {
		/* Set data pin to byte state */
		if ((bytes >> (7-i)) & 0x01)
			PORT_LCD |= (1 << LCD_DIN);
		else
			PORT_LCD &= ~(1 << LCD_DIN);

		/* Blink clock */
		PORT_LCD |= (1 << LCD_CLK);
		PORT_LCD &= ~(1 << LCD_CLK);
	}

	/* Disable controller */
	PORT_LCD |= (1 << LCD_SCE);
}

static void write_cmd(uint8_t cmd)
{
	write(cmd, 0);
}

static void write_data(uint8_t data)
{
	write(data, 1);
}

/*
 * Public functions
 */

void nokia_lcd_init(void)
{
	register unsigned i;
	/* Set pins as output */
	DDR_LCD |= (1 << LCD_SCE);
	DDR_LCD |= (1 << LCD_RST);
	DDR_LCD |= (1 << LCD_DC);
	DDR_LCD |= (1 << LCD_DIN);
	DDR_LCD |= (1 << LCD_CLK);

	/* Reset display */
	PORT_LCD |= (1 << LCD_RST);
	PORT_LCD |= (1 << LCD_SCE);
	_delay_ms(10);
	PORT_LCD &= ~(1 << LCD_RST);
	_delay_ms(70);
	PORT_LCD |= (1 << LCD_RST);

	/*
	 * Initialize display
	 */
	/* Enable controller */
	PORT_LCD &= ~(1 << LCD_SCE);
	/* -LCD Extended Commands mode- */
	write_cmd(0x21);
	/* LCD bias mode 1:48 */
	write_cmd(0x13);
	/* Set temperature coefficient */
	write_cmd(0x06);
	/* Default VOP (3.06 + 66 * 0.06 = 7V) */
	write_cmd(0xC2);
	/* Standard Commands mode, powered down */
	write_cmd(0x20);
	/* LCD in normal mode */
	write_cmd(0x09);

	/* Clear LCD RAM */
	write_cmd(0x80);
	write_cmd(LCD_CONTRAST);
	for (i = 0; i < 504; i++)
		write_data(0x00);

	/* Activate LCD */
	write_cmd(0x08);
	write_cmd(0x0C);

	/* Back-light */
	DDR_LCD |= (1 << LCD_LIGHT);
	PORT_LCD &= ~(1 << LCD_LIGHT);
}

void nokia_lcd_clear(void)
{
	register unsigned i;
	/* Set column and row to 0 */
	write_cmd(0x80);
	write_cmd(0x40);
	/*Cursor too */
	nokia_lcd.cursor_x = 0;
	nokia_lcd.cursor_y = 0;
	/* Clear everything (504 bytes = 84cols * 48 rows / 8 bits) */
	for(i = 0;i < 504; i++)
		nokia_lcd.screen[i] = 0x00;
}

void nokia_lcd_power(uint8_t on)
{
	write_cmd(on ? 0x20 : 0x24);
}

void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value)
{
	uint8_t *byte = &nokia_lcd.screen[y/8*84+x];
	if (value)
		*byte |= (1 << (y % 8));
	else
		*byte &= ~(1 << (y % 8));
}

void nokia_lcd_write_char(uint8_t code)
{
	register uint8_t x, y;

	for (x = 0; x < 3; x++)
		for (y = 0; y < 5; y++)
			if (pgm_read_byte(&CHARSET[code-32][x]) & (1 << y))
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, 1);
			else
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, 0);

	nokia_lcd.cursor_x += 3 + 1;
	if (nokia_lcd.cursor_x >= 84) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y += 5 + 1;
	}
	if (nokia_lcd.cursor_y >= 48) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y = 0;
	}
}

void nokia_lcd_write_string(const char *str)
{
	while(*str)
		nokia_lcd_write_char(*str++);
}

void nokia_lcd_set_cursor(uint8_t x, uint8_t y)
{
	nokia_lcd.cursor_x = x;
	nokia_lcd.cursor_y = y;
}

void nokia_lcd_render(void)
{
	register unsigned i;
	/* Set column and row to 0 */
	write_cmd(0x80);
	write_cmd(0x40);

	/* Write screen to display */
	for (i = 0; i < 504; i++)
		write_data(nokia_lcd.screen[i]);
}

// New Functions below:
void nokia_lcd_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value)
{
	register uint8_t i;

	if (x1 == x2) { // vertical line
		for (i = y1; i <= y2; ++i)
			nokia_lcd_set_pixel(x1, i, value);
	} else if (y1 == y2) { // horizontal line
		for (i = x1; i <= x2; ++i)
			nokia_lcd_set_pixel(i, y1, value);
	}
}

void nokia_lcd_draw_rect(uint8_t x, uint8_t y,
		uint8_t width, uint8_t height,
		uint8_t value, uint8_t fill)
{
	register uint8_t i;

	if (fill == RECT_NO_FILL) {
		nokia_lcd_draw_line(x, y, x + width - 1, y, value);
		nokia_lcd_draw_line(x, y + height - 1, x + width - 1, y + height - 1, value);
		nokia_lcd_draw_line(x, y, x, y + height - 1, value);
		nokia_lcd_draw_line(x + width - 1, y, x + width - 1, y + height - 1, value);
	} else if (fill == RECT_FILL) {
		for (i = y; i < y + height; ++i)
			nokia_lcd_draw_line(x, i, x + width - 1, i, value);
	}
}
