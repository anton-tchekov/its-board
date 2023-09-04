#include "shell.h"
#include "terminal.h"
#include "string.h"
#include "ps2.h"
#include "nanoc.h"
#include "error.h"
#include "stdio.h"
#include "command.h"

#define COMMAND_MAX_LENGTH  58

#define NORMAL                TERMINAL_COLOR(TERMINAL_WHITE, TERMINAL_BLACK)
#define CURSOR                TERMINAL_COLOR(TERMINAL_BLACK, TERMINAL_WHITE)
#define SELECTED              TERMINAL_COLOR(TERMINAL_BLACK, TERMINAL_RED)
#define CLEAR                 TERMINAL_CHAR(' ', NORMAL)

static char _clipboard[60];
static int _clipboard_len;

static char _command[60];
static int _length, _cursor, _selection;

static int _shell_fg, _shell_x, _shell_y;

/* --- PRIVATE --- */
static inline int _isprint(int c)
{
	return c >= 32 && c <= 126;
}

static void shell_output_reset(void)
{
	_shell_x = 0;
	_shell_y = 1;
	_shell_fg = TERMINAL_WHITE;
}

static void selection_get(int *start, int *end)
{
	if(_selection < _cursor)
	{
		*start = _selection;
		*end = _cursor;
	}
	else
	{
		*start = _cursor;
		*end = _selection;
	}
}

static void replace_selection(const char *s, int len)
{
	int start, end, sel_len, new_len;

	selection_get(&start, &end);
	sel_len = end - start;
	new_len = _length + len - sel_len;

	if(new_len > COMMAND_MAX_LENGTH)
	{
		return;
	}

	memmove(_command + start + len, _command + end, _length - end);
	memcpy(_command + start, s, len);

	_length = new_len;
	_cursor = start + len;
	_selection = _cursor;
}

static void delete_selection(void)
{
	int start, end;
	selection_get(&start, &end);
	memmove(_command + start, _command + end, _length - end);
	_length -= (end - start);
	_cursor = start;
	_selection = start;
}

static void copy_selection(void)
{
	int start, end;
	selection_get(&start, &end);
	_clipboard_len = end - start;
	memcpy(_clipboard, _command + start, _clipboard_len);
}

static void cls(void)
{
	int x, y;
	for(y = 1; y < TERMINAL_H; ++y)
	{
		for(x = 0; x < TERMINAL_W; ++x)
		{
			terminal_set(x, y, CLEAR);
		}
	}
}

static int get_char_at(int i, int start, int end)
{
	int color = NORMAL;
	int chr = ' ';

	if(i >= start && i < end)
	{
		color = SELECTED;
	}

	if(i == _cursor)
	{
		color = CURSOR;
	}

	if(i < _length)
	{
		chr = _command[i];
	}

	return TERMINAL_CHAR(chr, color);
}

static void command_render(void)
{
	int i, start, end;
	selection_get(&start, &end);
	terminal_set(0, 0, TERMINAL_CHAR('>', NORMAL));
	terminal_set(1, 0, TERMINAL_CHAR(' ', NORMAL));
	for(i = 0; i < COMMAND_MAX_LENGTH; ++i)
	{
		terminal_set(i + 2, 0, get_char_at(i, start, end));
	}
}

static void key_left(void)
{
	if(_cursor == _selection)
	{
		if(_cursor > 0)
		{
			--_cursor;
			_selection = _cursor;
		}
	}
	else
	{
		_cursor = (_cursor < _selection) ? _cursor : _selection;
		_selection = _cursor;
	}
}

static void key_shift_left(void)
{
	if(_cursor > 0)
	{
		--_cursor;
	}
}

static void key_right(void)
{
	if(_cursor == _selection)
	{
		if(_cursor < _length)
		{
			++_cursor;
			_selection = _cursor;
		}
	}
	else
	{
		_cursor = (_cursor > _selection) ? _cursor : _selection;
		_selection = _cursor;
	}
}

static void key_shift_right(void)
{
	if(_cursor < _length)
	{
		++_cursor;
	}
}

static void key_backspace(void)
{
	if(_selection == _cursor)
	{
		if(_cursor > 0)
		{
			char *p = _command + _cursor;
			memmove(p - 1, p, _length - _cursor);
			--_cursor;
			_selection = _cursor;
			--_length;
		}
	}
	else
	{
		delete_selection();
	}
}

static void key_delete(void)
{
	if(_selection == _cursor)
	{
		if(_cursor < _length)
		{
			char *p = _command + _cursor;
			--_length;
			memmove(p, p + 1, _length - _cursor);
		}
	}
	else
	{
		delete_selection();
	}
}

static void key_ctrl_a(void)
{
	_selection = 0;
	_cursor = _length;
}

static void key_ctrl_c(void)
{
	copy_selection();
}

static void key_ctrl_x(void)
{
	copy_selection();
	delete_selection();
}

static void key_ctrl_v(void)
{
	replace_selection(_clipboard, _clipboard_len);
}

static void key_home(void)
{
	_cursor = 0;
	_selection = _cursor;
}

static void key_shift_home(void)
{
	_cursor = 0;
}

static void key_end(void)
{
	_cursor = _length;
	_selection = _cursor;
}

static void key_shift_end(void)
{
	_cursor = _length;
}

static void key_enter(void)
{
	cls();
	shell_output_reset();
	command_run(_command, _length);
}

static void key_insert(int c)
{
	if(_selection == _cursor)
	{
		if(_length < COMMAND_MAX_LENGTH)
		{
			char *p = _command + _cursor;
			memmove(p + 1, p, _length - _cursor);
			_command[_cursor] = c;
			++_cursor;
			_selection = _cursor;
			++_length;
		}
	}
	else
	{
		char s[1] = { c };
		replace_selection(s, 1);
	}
}

static void shell_key(int key, int c)
{
	typedef struct KEYBIND
	{
		int Key;
		void (*Action)(void);
	} KeyBind;

	static const KeyBind keybinds[] =
	{
		{ KEY_LEFT,              key_left        },
		{ MOD_SHIFT | KEY_LEFT,  key_shift_left  },
		{ KEY_RIGHT,             key_right       },
		{ MOD_SHIFT | KEY_RIGHT, key_shift_right },
		{ KEY_BACKSPACE,         key_backspace   },
		{ KEY_DELETE,            key_delete      },
		{ MOD_CTRL | KEY_A,      key_ctrl_a      },
		{ MOD_CTRL | KEY_C,      key_ctrl_c      },
		{ MOD_CTRL | KEY_X,      key_ctrl_x      },
		{ MOD_CTRL | KEY_V,      key_ctrl_v      },
		{ KEY_HOME,              key_home        },
		{ MOD_SHIFT | KEY_HOME,  key_shift_home  },
		{ KEY_END,               key_end         },
		{ MOD_SHIFT | KEY_END,   key_shift_end   },
	};

	const KeyBind *kb = keybinds;
	const KeyBind *end = kb + ARRLEN(keybinds);
	for(; kb < end; ++kb)
	{
		if(key == kb->Key)
		{
			kb->Action();
			break;
		}
	}

	if(kb == end)
	{
		if(c == '\n')
		{
			key_enter();
		}
		else if(_isprint(c))
		{
			key_insert(c);
		}
	}

	command_render();
}

/* --- PUBLIC --- */
void shell_init(void)
{
	shell_output_reset();
	command_render();
}

void shell_update(void)
{
	KeyEvent event;
	if(!ps2_read(&event))
	{
		return;
	}

	if(event.State == KEYSTATE_RELEASED)
	{
		return;
	}

	shell_key(event.Key, event.Codepoint);
}

void shell_print(const char *s)
{
	int c;
	while((c = *s++))
	{
		shell_char(c);
	}
}

void shell_char(int c)
{
	if(c == '\n')
	{
		_shell_x = 0;
		++_shell_y;
	}
	else
	{
		if(_shell_y < TERMINAL_H)
		{
			int color = TERMINAL_COLOR(_shell_fg, TERMINAL_BLACK);
			terminal_set(_shell_x, _shell_y, TERMINAL_CHAR(c, color));
			++_shell_x;
			if(_shell_x >= TERMINAL_W)
			{
				_shell_x = 0;
				++_shell_y;
			}
		}
	}
}

void shell_fg(int color)
{
	_shell_fg = color;
}
