/**
 * @file    lcd.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 * @brief   LCD Routines
 */

#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define COLOR_RED      0xF800
#define COLOR_YELLOW   0xFFE0
#define COLOR_GREEN    0x07E0
#define COLOR_CYAN     0x07FF
#define COLOR_BLUE     0x001F
#define COLOR_MAGENTA  0xF81F
#define COLOR_WHITE    0xFFFF
#define COLOR_BLACK    0x0000

#define LCD_HEIGHT 320
#define LCD_WIDTH  480

enum LCD_Orientation
{
	L2R_U2D,
	L2R_D2U,
	R2L_U2D,
	R2L_D2U,
	U2D_L2R,
	U2D_R2L,
	D2U_L2R,
	D2U_R2L
};

void lcd_init(int orientation, int backlight, int color);
void lcd_set_backlight(int value);
int lcd_color(int r, int g, int b);
void lcd_rect(int x, int y, int w, int h, int color);

#endif /* __LCD_H__ */
