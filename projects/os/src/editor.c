#include "editor.h"
#include "line.h"
#include "terminal.h"
#include "keyboard.h"
#include "util.h"
#include "styles.h"
#include "font.h"

#include <stddef.h>
#include <ctype.h>
#include <string.h>

static char _buf[1024];
static Line line;
static int _page_y, _page_x;
static int _tab_size;
static int _show_space;

static void _set(uint16_t *temp, int x, int y, int c)
{
	y -= _page_y;
	x -= _page_x;
	if(x < 0 || y < 0 || x >= TERMINAL_W || y >= TERMINAL_H)
	{
		return;
	}

	temp[y * TERMINAL_W + x] = c;
}

static void page_shift_left(Line *line)
{
	if(_page_x > -5)
	{
		--_page_x;
	}
}

static void page_shift_right(Line *line)
{
	++_page_x;
}

static void page_shift_up(Line *line)
{
	if(_page_y > 0)
	{
		--_page_y;
	}
}

static void page_shift_down(Line *line)
{
	++_page_y;
}

static void toggle_tab_size(Line *line)
{
	_tab_size <<= 1;
	if(_tab_size > 8)
	{
		_tab_size = 2;
	}
}

static void toggle_show_space(Line *line)
{
	_show_space = !_show_space;
}

static int _tab(uint16_t *temp, int x, int y, int color)
{
	int n = x & (_tab_size - 1);
	if(_show_space)
	{
		if(n == _tab_size - 1)
		{
			_set(temp, x, y, TERMINAL_CHAR(CHAR_TAB_BOTH, color));
			++x;
		}
		else
		{
			_set(temp, x, y, TERMINAL_CHAR(CHAR_TAB_START, color));
			++x;
			for(++n; n < _tab_size - 1; ++n)
			{
				_set(temp, x, y, TERMINAL_CHAR(CHAR_TAB_MIDDLE, color));
				++x;
			}
			_set(temp, x, y, TERMINAL_CHAR(CHAR_TAB_END, color));
			++x;
		}
	}
	else
	{
		for(; n < _tab_size; ++n)
		{
			_set(temp, x, y, TERMINAL_CHAR(' ', color));
			++x;
		}
	}

	return x;
}

static void linenumber(uint16_t *temp, int y)
{
	int x = -2;
	int n = y + 1;
	do
	{
		int c = n % 10 + '0';
		_set(temp, x, y, TERMINAL_CHAR(c, NORMAL));
		n /= 10;
		--y;
	}
	while(n > 0);
}

static void editor_render(void)
{
	uint16_t temp[TERMINAL_W * TERMINAL_H];
	int i, x, y, c, len, start, end, color;
	const char *text;

	line_selection_get(&line, &start, &end);

	memset(temp, 0, sizeof(temp));
	x = 0;
	y = 0;

	len = line.Length;
	_buf[len++] = '\n';

	text = _buf;
	for(i = 0; i < len; ++i)
	{
		if(x == 0)
		{
			linenumber(temp, y);
		}

		c = text[i];
		if(i == line.Cursor)
		{
			color = CURSOR;
		}
		else if(i >= start && i < end)
		{
			color = SELECTED;
		}
		else
		{
			color = NORMAL;
		}

		if(c == '\n')
		{
			_set(temp, x, y, TERMINAL_CHAR(' ', color));
			x = 0;
			++y;
		}
		else if(c == '\t')
		{
			x = _tab(temp, x, y, color);
		}
		else
		{
			if(_show_space && c == ' ')
			{
				c = CHAR_VISIBLE_SPACE;
			}

			_set(temp, x, y, TERMINAL_CHAR(c, color));
			++x;
		}
	}

	i = 0;
	for(y = 0; y < TERMINAL_H; ++y)
	{
		for(x = 0; x < TERMINAL_W; ++x, ++i)
		{
			terminal_set(x, y, temp[i]);
		}
	}
}

void editor_open(const char *filename)
{
	_tab_size = 4;
	_show_space = 1;
	line_init(&line, _buf, 1024);
	editor_render();
}

void editor_key(int key, int c)
{
	typedef struct KEYBIND
	{
		int Key;
		void (*Action)(Line *);
	} KeyBind;

	static const KeyBind keybinds[] =
	{
		{ KEY_LEFT,              line_key_left        },
		{ MOD_SHIFT | KEY_LEFT,  line_key_shift_left  },
		{ MOD_CTRL | KEY_LEFT,   page_shift_left      },

		{ KEY_RIGHT,             line_key_right       },
		{ MOD_SHIFT | KEY_RIGHT, line_key_shift_right },
		{ MOD_CTRL | KEY_RIGHT,  page_shift_right     },

		{ KEY_UP,                line_key_up          },
		{ MOD_SHIFT | KEY_UP,    line_key_shift_up    },
		{ MOD_CTRL | KEY_UP,     page_shift_up        },

		{ KEY_DOWN,              line_key_down        },
		{ MOD_SHIFT | KEY_DOWN,  line_key_shift_down  },
		{ MOD_CTRL | KEY_DOWN,   page_shift_down      },

		{ KEY_BACKSPACE,         line_key_backspace   },
		{ KEY_DELETE,            line_key_delete      },

		{ MOD_CTRL | KEY_A,      line_key_ctrl_a      },
		{ MOD_CTRL | KEY_C,      line_key_ctrl_c      },
		{ MOD_CTRL | KEY_X,      line_key_ctrl_x      },
		{ MOD_CTRL | KEY_V,      line_key_ctrl_v      },

		{ KEY_HOME,              line_key_home        },
		{ MOD_SHIFT | KEY_HOME,  line_key_shift_home  },
		{ KEY_END,               line_key_end         },
		{ MOD_SHIFT | KEY_END,   line_key_shift_end   },

		{ MOD_CTRL | KEY_T,      toggle_tab_size      },
		{ MOD_CTRL | KEY_J,      toggle_show_space    },

		{ MOD_CTRL | KEY_HOME,             line_key_ctrl_home        },
		{ MOD_CTRL | MOD_SHIFT | KEY_HOME, line_key_ctrl_shift_home  },
		{ MOD_CTRL | KEY_END,              line_key_ctrl_end         },
		{ MOD_CTRL | MOD_SHIFT | KEY_END,  line_key_ctrl_shift_end   },
	};

	const KeyBind *kb = keybinds;
	const KeyBind *end = kb + ARRLEN(keybinds);
	for(; kb < end; ++kb)
	{
		if(key == kb->Key)
		{
			kb->Action(&line);
			goto render;
		}
	}

	if(c == '\n' || c == '\t' || isprint(c))
	{
		line_key_insert(&line, c);
	}

render:
	editor_render();
}
