#include <stdio.h>
#include <ctype.h>

#include "line.h"
#include "shell.h"
#include "util.h"
#include "terminal.h"
#include "ps2.h"
#include "command.h"
#include "editor.h"
#include "styles.h"

#define COMMAND_MAX_LENGTH  57
#define COMMAND_BUFFER_SIZE 60
#define COMMAND_LINES        4

#define CLEAR                 TERMINAL_CHAR(' ', NORMAL)

typedef struct
{
	int Slot;
	int X, Y;
	int FG;
	Line Lines[COMMAND_LINES];
} Shell;

static char _bufs[COMMAND_LINES][COMMAND_BUFFER_SIZE];
static Shell this;

/* --- UTILS --- */
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

/* --- PRIVATE --- */
static void shell_output_reset(void)
{
	this.X = 0;
	this.Y = 1;
	this.FG = TERMINAL_WHITE;
}

static void key_enter(Line *line)
{
	cls();
	shell_output_reset();
	line->Text[line->Length] = '\0';
	command_run(line->Text, line->Length);
}

/* --- RENDER --- */
static inline int get_char_at(Line *line, int i, int start, int end)
{
	int color = NORMAL;
	int chr = ' ';

	if(i >= start && i < end)
	{
		color = SELECTED;
	}

	if(i == line->Cursor)
	{
		color = CURSOR;
	}

	if(i < line->Length)
	{
		chr = line->Text[i];
	}

	return TERMINAL_CHAR(chr, color);
}

static void command_render(void)
{
	static const uint8_t _colors[COMMAND_LINES] =
	{
		TERMINAL_BRIGHT_WHITE,
		TERMINAL_BRIGHT_RED,
		TERMINAL_BRIGHT_GREEN,
		TERMINAL_BRIGHT_BLUE
	};

	Line *line = &this.Lines[this.Slot];
	int i, start, end;
	int color = TERMINAL_COLOR(_colors[this.Slot], TERMINAL_BLACK);
	line_selection_get(line, &start, &end);
	terminal_set(0, 0, TERMINAL_CHAR('>', color));
	terminal_set(1, 0, TERMINAL_CHAR(' ', NORMAL));
	for(i = 0; i < COMMAND_MAX_LENGTH + 1; ++i)
	{
		terminal_set(i + 2, 0, get_char_at(line, i, start, end));
	}
}

static void key_up(Line *line)
{
	if(this.Slot == 0)
	{
		this.Slot = COMMAND_LINES;
	}

	--this.Slot;
	(void)line;
}

static void key_down(Line *line)
{
	++this.Slot;
	if(this.Slot >= COMMAND_LINES)
	{
		this.Slot = 0;
	}
	(void)line;
}

static void shell_key(int key, int c)
{
	typedef struct KEYBIND
	{
		int Key;
		void (*Action)(Line *);
	} KeyBind;

	static const KeyBind keybinds[] =
	{
		{ KEY_LEFT,              line_key_left            },
		{ MOD_SHIFT | KEY_LEFT,  line_key_shift_left      },
		{ KEY_RIGHT,             line_key_right           },
		{ MOD_SHIFT | KEY_RIGHT, line_key_shift_right     },
		{ KEY_UP,                key_up                   },
		{ KEY_DOWN,              key_down                 },
		{ KEY_BACKSPACE,         line_key_backspace       },
		{ KEY_DELETE,            line_key_delete          },
		{ MOD_CTRL | KEY_A,      line_key_ctrl_a          },
		{ MOD_CTRL | KEY_C,      line_key_ctrl_c          },
		{ MOD_CTRL | KEY_X,      line_key_ctrl_x          },
		{ MOD_CTRL | KEY_V,      line_key_ctrl_v          },
		{ KEY_HOME,              line_key_ctrl_home       },
		{ MOD_SHIFT | KEY_HOME,  line_key_ctrl_shift_home },
		{ KEY_END,               line_key_ctrl_end        },
		{ MOD_SHIFT | KEY_END,   line_key_ctrl_shift_end  },
	};

	Line *line = &this.Lines[this.Slot];
	const KeyBind *kb = keybinds;
	const KeyBind *end = kb + ARRLEN(keybinds);
	for(; kb < end; ++kb)
	{
		if(key == kb->Key)
		{
			kb->Action(line);
			goto render;
		}
	}

	if(c == '\n')
	{
		key_enter(line);
	}
	else if(isprint(c))
	{
		line_key_insert(line, c);
	}

render:
	command_render();
}

/* --- PUBLIC --- */
void shell_init(void)
{
	int i;

	shell_output_reset();
	command_render();

	for(i = 0; i < COMMAND_LINES; ++i)
	{
		line_init(&this.Lines[i], _bufs[i], COMMAND_MAX_LENGTH);
	}
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

	editor_key(event.Key, event.Codepoint);

	//shell_key(event.Key, event.Codepoint);
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
		this.X = 0;
		++this.Y;
	}
	else
	{
		if(this.Y < TERMINAL_H)
		{
			int color = TERMINAL_COLOR(this.FG, TERMINAL_BLACK);
			terminal_set(this.X, this.Y, TERMINAL_CHAR(c, color));
			++this.X;
			if(this.X >= TERMINAL_W)
			{
				this.X = 0;
				++this.Y;
			}
		}
	}
}

void shell_fg(int color)
{
	this.FG = color;
}
