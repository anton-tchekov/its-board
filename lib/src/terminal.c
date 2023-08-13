#include "lcd.h"
#include "terminal.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"

#define TERMINAL_CHARS 1200

static uint16_t terminal_buf[TERMINAL_CHARS], terminal_cur[TERMINAL_CHARS];
static int terminal_x, terminal_y, terminal_w, terminal_h,
	char_w, char_h, terminal_style;

#define CT_BLACK          0x0000
#define CT_RED            0xA800
#define CT_GREEN          0x0540
#define CT_YELLOW         0xAAA0
#define CT_BLUE           0x0015
#define CT_MAGENTA        0xA815
#define CT_CYAN           0x0555
#define CT_WHITE          0xAD55

#define CT_BRIGHT_BLACK   0x52AA
#define CT_BRIGHT_RED     0xFAAA
#define CT_BRIGHT_GREEN   0x57EA
#define CT_BRIGHT_YELLOW  0xFFEA
#define CT_BRIGHT_BLUE    0x52BF
#define CT_BRIGHT_MAGENTA 0xFABF
#define CT_BRIGHT_CYAN    0x57FF
#define CT_BRIGHT_WHITE   0xFFFF

static const uint16_t color_table[] =
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

static inline int is_bold(int v)
{
	return v & 0x800;
}

static inline int extract_char(int v)
{
	return v & 0xFF;
}

static inline int extract_fg_id(int v)
{
	return (v >> 8) & 0x0F;
}

static inline int extract_bg_id(int v)
{
	return v >> 12;
}

static inline int extract_fg(int v)
{
	return color_table[extract_fg_id(v)];
}

static inline int extract_bg(int v)
{
	return color_table[extract_bg_id(v)];
}

static inline int style(int fg, int bg)
{
	return fg | (bg << 4);
}

static inline int combine(int c, int style)
{
	return c | (style << 8);
}

static void terminal_set(int x, int y, int v)
{
	terminal_buf[y * terminal_w + x] = v;
}

static int terminal_get(int x, int y)
{
	return terminal_buf[y * terminal_w + x];
}

static void terminal_render(int x, int y, int v)
{
	font_char(x * char_w, y * char_h, extract_char(v),
		extract_fg(v), extract_bg(v),
		is_bold(v) ? Terminus16_Bold : Terminus16);
}

static void terminal_update(void)
{
	int x, y, v, c, fg, bg;
	for(y = 0; y < terminal_h; ++y)
	{
		for(x = 0; x < terminal_w; ++x)
		{
			v = terminal_get(x, y);
			c = extract_char(v);
			fg = extract_fg_id(v);
			bg = extract_bg_id(v);

			if(x == terminal_x && y == terminal_y)
			{
				v = combine(c, style(bg, fg));
			}

			if(terminal_cur[y * terminal_w + x] != v)
			{
				terminal_render(x, y, v);
				terminal_cur[y * terminal_w + x] = v;
			}
		}
	}
}

static void terminal_clear_line(int line)
{
	int x, v;
	v = combine(' ', terminal_style);
	for(x = 0; x < terminal_w; ++x)
	{
		terminal_set(x, line, v);
	}
}

static void terminal_scroll_up(void)
{
	int x, y;
	for(y = 0; y < terminal_h - 1; ++y)
	{
		for(x = 0; x < terminal_w; ++x)
		{
			terminal_set(x, y, terminal_get(x, y + 1));
		}
	}

	terminal_clear_line(terminal_h - 1);
}

static void terminal_newline(void)
{
	terminal_x = 0;
	if(++terminal_y == terminal_h)
	{
		terminal_scroll_up();
		terminal_y = terminal_h - 1;
	}
}

static void _terminal_char(int c)
{
	if(c == '\n')
	{
		terminal_newline();
	}
	else
	{
		terminal_set(terminal_x, terminal_y, combine(c, terminal_style));
		if(++terminal_x == terminal_w)
		{
			terminal_newline();
		}
	}
}

/* --- PUBLIC --- */
void terminal_clear(void)
{
	int x, y, v;
	v = combine(' ', terminal_style);
	for(y = 0; y < terminal_h; ++y)
	{
		for(x = 0; x < terminal_w; ++x)
		{
			terminal_set(x, y, v);
		}
	}

	terminal_x = 0;
	terminal_y = 0;
	terminal_update();
}

void terminal_fg(int color)
{
	terminal_style &= 0xF0;
	terminal_style |= color;
}

void terminal_char(int c)
{
	_terminal_char(c);
	terminal_update();
}

void terminal_init(void)
{
	char_w = 8;
	char_h = 16;
	terminal_w = 60;
	terminal_h = 20;
	terminal_style = style(ID_WHITE, ID_BLACK);
	terminal_clear();
}

void terminal_print(const char *s)
{
	int c;
	while((c = *s++))
	{
		_terminal_char(c);
	}

	terminal_update();
}

void terminal_xy(int x, int y)
{
	terminal_x = x;
	terminal_y = y;
	terminal_update();
}
