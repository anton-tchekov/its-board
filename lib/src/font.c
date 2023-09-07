#include "font.h"
#include "lcd.h"

void font_char(int x, int y, int c, int fg, int bg, const Font *font)
{
	if(c < 32)
	{
		c = 127;
	}

	lcd_window_start(x, y, font->Width, font->Height);
	int stride = (font->Width + 7) >> 3;
	const uint8_t *char_bitmap = &font->Bitmap[(c - 32) * font->Height * stride];
	for(int y = 0; y < font->Height; ++y)
	{
		for(int x = 0; x < font->Width; ++x)
		{
			int byte = (y * stride) + (x >> 3);
			int bit = 1 << (7 - (x & 0x7));
			lcd_emit(char_bitmap[byte] & bit ? fg : bg);
		}
	}

	lcd_window_end();
}

void font_str(int x, int y, const char *s, int fg, int bg, const Font *font)
{
	int c;
	while((c = *s++))
	{
		font_char(x, y, c, fg, bg, font);
		x += font->Width;
	}
}
