#include "stdio.h"
#include "string.h"
#include "ctype.h"

#include "shell.h"
#include "terminal.h"
#include "ps2.h"
#include "nanoc.h"
#include "error.h"
#include "command.h"
#include "clipboard.h"

#define COMMAND_MAX_LENGTH  57
#define COMMAND_BUFFER_SIZE 60
#define COMMAND_LINES        4

#define NORMAL                TERMINAL_COLOR(TERMINAL_WHITE, TERMINAL_BLACK)
#define CURSOR                TERMINAL_COLOR(TERMINAL_BLACK, TERMINAL_WHITE)
#define SELECTED              TERMINAL_COLOR(TERMINAL_BLACK, TERMINAL_YELLOW)
#define CLEAR                 TERMINAL_CHAR(' ', NORMAL)

typedef struct
{
	int Length;
	int Cursor;
	int Selection;
	char Text[COMMAND_BUFFER_SIZE];
} Line;

typedef struct
{
	int Slot;
	int X, Y;
	int FG;
	Line Lines[COMMAND_LINES];
} Shell;

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

/* --- SELECTION --- */
static void selection_get(Line *line, int *start, int *end)
{
	if(line->Selection < line->Cursor)
	{
		*start = line->Selection;
		*end = line->Cursor;
	}
	else
	{
		*start = line->Cursor;
		*end = line->Selection;
	}
}

static void selection_replace(Line *line, const char *s, int len)
{
	int start, end, sel_len, new_len;
	selection_get(line, &start, &end);
	sel_len = end - start;
	new_len = line->Length + len - sel_len;
	if(new_len > COMMAND_MAX_LENGTH)
	{
		return;
	}

	memmove(line->Text + start + len, line->Text + end, line->Length - end);
	memcpy(line->Text + start, s, len);
	line->Length = new_len;
	line->Cursor = start + len;
	line->Selection = line->Cursor;
}

static void selection_delete(Line *line)
{
	int start, end;
	selection_get(line, &start, &end);
	memmove(line->Text + start, line->Text + end, line->Length - end);
	line->Length -= (end - start);
	line->Cursor = start;
	line->Selection = start;
}

static void selection_copy(Line *line)
{
	int start, end;
	selection_get(line, &start, &end);
	clipboard_save(line->Text + start, end - start);
}

/* --- LINE EDIT --- */
static void key_left(Line *line)
{
	if(line->Cursor == line->Selection)
	{
		if(line->Cursor > 0)
		{
			--line->Cursor;
			line->Selection = line->Cursor;
		}
	}
	else
	{
		line->Cursor = (line->Cursor < line->Selection)
			? line->Cursor : line->Selection;
		line->Selection = line->Cursor;
	}
}

static void key_shift_left(Line *line)
{
	if(line->Cursor > 0)
	{
		--line->Cursor;
	}
}

static void key_right(Line *line)
{
	if(line->Cursor == line->Selection)
	{
		if(line->Cursor < line->Length)
		{
			++line->Cursor;
			line->Selection = line->Cursor;
		}
	}
	else
	{
		line->Cursor = (line->Cursor > line->Selection)
			? line->Cursor : line->Selection;
		line->Selection = line->Cursor;
	}
}

static void key_shift_right(Line *line)
{
	if(line->Cursor < line->Length)
	{
		++line->Cursor;
	}
}

static void key_backspace(Line *line)
{
	if(line->Selection == line->Cursor)
	{
		if(line->Cursor > 0)
		{
			char *p = line->Text + line->Cursor;
			memmove(p - 1, p, line->Length - line->Cursor);
			--line->Cursor;
			line->Selection = line->Cursor;
			--line->Length;
		}
	}
	else
	{
		selection_delete(line);
	}
}

static void key_delete(Line *line)
{
	if(line->Selection == line->Cursor)
	{
		if(line->Cursor < line->Length)
		{
			char *p = line->Text + line->Cursor;
			--line->Length;
			memmove(p, p + 1, line->Length - line->Cursor);
		}
	}
	else
	{
		selection_delete(line);
	}
}

static void key_ctrl_a(Line *line)
{
	line->Selection = 0;
	line->Cursor = line->Length;
}

static void key_ctrl_c(Line *line)
{
	selection_copy(line);
}

static void key_ctrl_x(Line *line)
{
	selection_copy(line);
	selection_delete(line);
}

static void key_ctrl_v(Line *line)
{
	int len;
	const char *text = clipboard_get(&len);
	selection_replace(line, text, len);
}

static void key_home(Line *line)
{
	line->Cursor = 0;
	line->Selection = line->Cursor;
}

static void key_shift_home(Line *line)
{
	line->Cursor = 0;
}

static void key_end(Line *line)
{
	line->Cursor = line->Length;
	line->Selection = line->Cursor;
}

static void key_shift_end(Line *line)
{
	line->Cursor = line->Length;
}

static void key_enter(Line *line)
{
	cls();
	shell_output_reset();
	command_run(line->Text, line->Length);
}

static void key_insert(Line *line, int c)
{
	if(line->Selection == line->Cursor)
	{
		if(line->Length < COMMAND_MAX_LENGTH)
		{
			char *p = line->Text + line->Cursor;
			memmove(p + 1, p, line->Length - line->Cursor);
			line->Text[line->Cursor] = c;
			++line->Cursor;
			line->Selection = line->Cursor;
			++line->Length;
		}
	}
	else
	{
		char s[1] = { c };
		selection_replace(line, s, 1);
	}
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
	selection_get(line, &start, &end);
	terminal_set(0, 0, TERMINAL_CHAR('>', color));
	terminal_set(1, 0, TERMINAL_CHAR(' ', NORMAL));
	for(i = 0; i < COMMAND_MAX_LENGTH; ++i)
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
		{ KEY_LEFT,              key_left        },
		{ MOD_SHIFT | KEY_LEFT,  key_shift_left  },
		{ KEY_RIGHT,             key_right       },
		{ MOD_SHIFT | KEY_RIGHT, key_shift_right },
		{ KEY_UP,                key_up          },
		{ KEY_DOWN,              key_down        },
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

	Line *line = &this.Lines[this.Slot];
	const KeyBind *kb = keybinds;
	const KeyBind *end = kb + ARRLEN(keybinds);
	for(; kb < end; ++kb)
	{
		if(key == kb->Key)
		{
			kb->Action(line);
			break;
		}
	}

	if(kb == end)
	{
		if(c == '\n')
		{
			key_enter(line);
		}
		else if(isprint(c))
		{
			key_insert(line, c);
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
