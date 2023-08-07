/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 * @brief   Template Project
 */

#include "init.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "font.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"
#include "ps2.h"

#define PIN 4

// Variante B
int draw_chessboard(int x, int y, void *handle)
{
	if(((x / 8) & 1) == ((y / 8) & 1))
	{
		return COLOR_WHITE;
	}

	return COLOR_BLACK;

	(void)handle;
}

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	int cx = 30;

	its_board_init();
	timer_init();
	ps2_init();
	lcd_init(D2U_L2R, 1000, COLOR_BLACK);
	__enable_irq();


	lcd_rect(20, 20, 80, 20, COLOR_RED);
	font_str(100, 10, "Hello World", COLOR_WHITE, COLOR_BLACK, Terminus16);
	font_str(100, 20, "Hello World Bold", COLOR_WHITE, COLOR_BLACK, Terminus16_Bold);

	// Variante A - Explizit
	{
		int x, y;
		int w = LCD_WIDTH - 100;
		int h = LCD_HEIGHT - 100;
		lcd_window_start(50, 50, w, h);
		for(y = 0; y < h; ++y)
		{
			for(x = 0; x < w; ++x)
			{
				lcd_emit(lcd_color(x * 255 / w, y * 255 / h, 0));
			}
		}
		lcd_window_end();
	}

	// Variante B - Mit Callback
	lcd_callback(LCD_WIDTH / 2 - 40, LCD_HEIGHT / 2 - 40, 80, 80,
		NULL, draw_chessboard);

	KeyEvent event;
	for(;;)
	{
		if(ps2_read(&event))
		{
			font_char(cx, 50, event.Codepoint,
				COLOR_WHITE, COLOR_BLACK, Terminus16);

			cx += 8;
		}

	}

	return 0;
}
