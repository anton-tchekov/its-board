#include "terminal.h"

#include "lcd.h"
#include "font.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"

#define TERMINAL_CHAR_W            8
#define TERMINAL_CHAR_H           16

#define TERMINAL_EX_CHAR(V)         ((V) & 0xFF)
#define TERMINAL_EX_FG(V)           (((V) >> 8) & 0x0F)
#define TERMINAL_EX_BG(V)           ((V) >> 12)
#define TERMINAL_EX_BOLD(V)         ((V) & 0x800)

#define CT_BLACK              0x0000
#define CT_RED                0xA800
#define CT_GREEN              0x0540
#define CT_YELLOW             0xAAA0
#define CT_BLUE               0x0015
#define CT_MAGENTA            0xA815
#define CT_CYAN               0x0555
#define CT_WHITE              0xAD55

#define CT_BRIGHT_BLACK       0x52AA
#define CT_BRIGHT_RED         0xFAAA
#define CT_BRIGHT_GREEN       0x57EA
#define CT_BRIGHT_YELLOW      0xFFEA
#define CT_BRIGHT_BLUE        0x52BF
#define CT_BRIGHT_MAGENTA     0xFABF
#define CT_BRIGHT_CYAN        0x57FF
#define CT_BRIGHT_WHITE       0xFFFF

static const uint16_t _color_table[] =
{
	CT_BLACK,
	CT_RED,
	CT_GREEN,
	CT_YELLOW,
	CT_BLUE,
	CT_MAGENTA,
	CT_CYAN,
	CT_WHITE,

	CT_BRIGHT_BLACK,
	CT_BRIGHT_RED,
	CT_BRIGHT_GREEN,
	CT_BRIGHT_YELLOW,
	CT_BRIGHT_BLUE,
	CT_BRIGHT_MAGENTA,
	CT_BRIGHT_CYAN,
	CT_BRIGHT_WHITE
};

static uint16_t _terminal_buf[TERMINAL_W * TERMINAL_H];

static inline void _terminal_render(int x, int y, int v)
{
	font_char(
		x * TERMINAL_CHAR_W,
		y * TERMINAL_CHAR_H,
		TERMINAL_EX_CHAR(v),
		_color_table[TERMINAL_EX_FG(v)],
		_color_table[TERMINAL_EX_BG(v)],
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
