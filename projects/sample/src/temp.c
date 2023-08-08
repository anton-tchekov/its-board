#if 0

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


	int cx = 30;


	font_str(100, 10, "Hello World\xFF\xFF\xFF", COLOR_WHITE, COLOR_BLACK, Terminus16);
	font_str(100, 30, "Hello World Bold\xFF\xFF", COLOR_WHITE, COLOR_BLACK, Terminus16_Bold);

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

	uint32_t t0 = timer_get();

	// Variante B - Mit Callback
	lcd_callback(LCD_WIDTH / 2 - 40, LCD_HEIGHT / 2 - 40, 80, 80,
		NULL, draw_chessboard);

	uint32_t t1 = timer_get();
	char buf[32];

	font_str(10, 80, "Time Delta to draw Chessboard:",
		COLOR_BLACK, COLOR_WHITE, Terminus16);

	timer_delta_str(buf, t1 - t0);

	font_str(50, 100, buf,
		COLOR_BLACK, COLOR_WHITE, Terminus16);


/*	draw_fancy_logo(LCD_WIDTH / 2 - LOGO_WIDTH / 2,
		LCD_HEIGHT / 2 - LOGO_HEIGHT / 2,
		LOGO_WIDTH, LOGO_HEIGHT, 0);*/


#define LOGO_WIDTH   160
#define LOGO_HEIGHT  200

#endif