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

#define PIN 4

// Variante B
int draw_chessboard(int x, int y, int handle)
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
	its_board_init();
	timer_init();
	lcd_init(D2U_L2R, 1000, COLOR_BLACK);
	lcd_rect(20, 20, 80, 20, COLOR_RED);

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
		0, draw_chessboard);

	for(;;)
	{
		GPIOD->BSRR = (1 << PIN);
		delay_ms(200);
		GPIOD->BSRR = (1 << (PIN + 16));
		delay_ms(200);
		GPIOD->BSRR = (1 << PIN);
		delay_ms(200);
		GPIOD->BSRR = (1 << (PIN + 16));
		delay_ms(1000);

	}

	return 0;
}
