#include "terminal.h"

#include "lcd.h"
#include "font.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"

#define TERMINAL_CHAR_W            8
#define TERMINAL_CHAR_H           16

#define TERMINAL_EX_CHAR(V)         ((V) & 0xFF)
#define TERMINAL_EX_FG(V)           (((V) >> 8) & 0x0F)
#define TERMINAL_EX_BG(V)           (((V) >> 12) & 0x07)
#define TERMINAL_EX_BOLD(V)         ((V) & 0x8000)

static const uint16_t _color_table_bg[] =
{
	0x0000, /* TERMINAL_BG_BLACK   */
	0xFFFF, /* TERMINAL_BG_WHITE   */
	0xFC00, /* TERMINAL_BG_ORANGE  */
	0x226F, /* TERMINAL_BG_BLUE    */
};

static const uint16_t _color_table_fg[] =
{
	0x0000, /* TERMINAL_FG_BLACK        */
	0xFFFF, /* TERMINAL_FG_WHITE        */
	0xFAAA, /* TERMINAL_FG_RED          */
	0x57EA, /* TERMINAL_FG_GREEN        */
	0x52BF, /* TERMINAL_FG_BLUE         */
	0x632C, /* TERMINAL_FG_LIGHT_GRAY   */
	0xDB9A, /* TERMINAL_FG_MAGENTA      */
	0xFEA0, /* TERMINAL_FG_YELLOW       */
	0xDEF5, /* TERMINAL_FG_LIGHT_YELLOW */
	0x14FF, /* TERMINAL_FG_LIGHT_BLUE   */
	0xB675, /* TERMINAL_FG_LIGHT_GREEN  */
	0xCC8F, /* TERMINAL_FG_ORANGE       */
	0x6CCA, /* TERMINAL_FG_DARK_GREEN   */
	0x54FA, /* TERMINAL_FG_DARK_BLUE    */
};

static uint16_t _terminal_buf[TERMINAL_W * TERMINAL_H];

static inline void _terminal_render(int x, int y, int v)
{
	font_char(
		x * TERMINAL_CHAR_W,
		y * TERMINAL_CHAR_H,
		TERMINAL_EX_CHAR(v),
		_color_table_fg[TERMINAL_EX_FG(v)],
		_color_table_bg[TERMINAL_EX_BG(v)],
		TERMINAL_EX_BOLD(v) ? Terminus16_Bold : Terminus16);
}

void terminal_set(int x, int y, int v)
{
	uint16_t *p = _terminal_buf + y * TERMINAL_W + x;
	if(v != *p)
	{
		*p = v;
		_terminal_render(x, y, v);
	}
}

void terminal_clear(void)
{
	int x, y;
	for(y = 0; y < TERMINAL_H; ++y)
	{
		for(x = 0; x < TERMINAL_W; ++x)
		{
			terminal_set(x, y, ' ' | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
		}
	}
}
