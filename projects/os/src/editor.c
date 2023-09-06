#include "editor.h"
#include "line.h"
#include "terminal.h"
#include "keyboard.h"
#include "util.h"
#include "styles.h"

#include <stddef.h>
#include <ctype.h>
#include <string.h>

static char _buf[1024];
static Line line;
static int _page_y;

static void _set(uint16_t *temp, int x, int y, int c)
{
	y -= _page_y;
	if(x < 0 || y < 0 || x >= TERMINAL_W || y >= TERMINAL_H)
	{
		return;
	}

	temp[y * TERMINAL_W + x] = c;
}

static void editor_render(void)
{
	uint16_t temp[TERMINAL_W * TERMINAL_H];
	int i, x, y, c, d, len, start, end, color;
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

		d = c == '\n' ? ' ' : c;
		_set(temp, x, y, TERMINAL_CHAR(d, color));
		if(c == '\n')
		{
			x = 0;
			++y;
		}
		else
		{
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
	line_init(&line, _buf, 1024);
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
		{ KEY_RIGHT,             line_key_right       },
		{ MOD_SHIFT | KEY_RIGHT, line_key_shift_right },
		{ KEY_UP,                line_key_up          },
		{ MOD_SHIFT | KEY_UP,    line_key_shift_up    },
		{ KEY_DOWN,              line_key_down        },
		{ MOD_SHIFT | KEY_DOWN,  line_key_shift_down  },
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

	if(c == '\n' || isprint(c))
	{
		line_key_insert(&line, c);
	}

render:
	editor_render();
}


