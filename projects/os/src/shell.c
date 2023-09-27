#include <stdio.h>
#include <ctype.h>

#include "line.h"
#include "shell.h"
#include "util.h"
#include "terminal.h"
#include "ps2.h"
#include "command.h"
#include "editor.h"

#define COMMAND_MAX_LENGTH  57
#define COMMAND_BUFFER_SIZE 60
#define COMMAND_LINES        4

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
			terminal_set(x, y, ' ' | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
		}
	}
}

/* --- PRIVATE --- */
static void shell_output_reset(void)
{
	this.X = 0;
	this.Y = 1;
	this.FG = TERMINAL_FG_WHITE;
}

static void key_enter(Line *line)
{
	cls();
	shell_output_reset();
	line->Text[line->Length] = '\0';
	command_run(line->Text, line->Length);
}

/* --- RENDER --- */
static inline void put_char_at(Line *line, int i, int start, int end)
{
	int color = TERMINAL_FG_WHITE | TERMINAL_BG_BLACK;
	int chr = ' ';

	if(i >= start && i < end)
	{
		color = TERMINAL_FG_BLACK | TERMINAL_BG_ORANGE;
	}

	if(i == line->Cursor)
	{
		color = TERMINAL_FG_BLACK | TERMINAL_BG_WHITE;
	}

	if(i < line->Length)
	{
		chr = line->Text[i];
	}

	terminal_set(i + 2, 0, chr | color);
}

static void command_render(void)
{
	static const uint16_t _colors[COMMAND_LINES] =
	{
		TERMINAL_FG_WHITE,
		TERMINAL_FG_RED,
		TERMINAL_FG_GREEN,
		TERMINAL_FG_BLUE
	};

	Line *line = &this.Lines[this.Slot];
	int i, start, end;
	int color = _colors[this.Slot] | TERMINAL_BG_BLACK;
	line_selection_get(line, &start, &end);
	terminal_set(0, 0, '>' | color);
	terminal_set(1, 0, ' ' | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
	for(i = 0; i < COMMAND_MAX_LENGTH + 1; ++i)
	{
		put_char_at(line, i, start, end);
	}
}

static void key_up(void)
{
	if(this.Slot == 0)
	{
		this.Slot = COMMAND_LINES;
	}

	--this.Slot;
}

static void key_down(void)
{
	++this.Slot;
	if(this.Slot >= COMMAND_LINES)
	{
		this.Slot = 0;
	}
}

void shell_key(int key, int c)
{
	Line *line = &this.Lines[this.Slot];
	switch(key)
	{
	case KEY_LEFT:              line_left(line);            break;
	case MOD_SHIFT | KEY_LEFT:  line_shift_left(line);      break;
	case KEY_RIGHT:             line_right(line);           break;
	case MOD_SHIFT | KEY_RIGHT: line_shift_right(line);     break;
	case KEY_UP:                key_up();                       break;
	case KEY_DOWN:              key_down();                     break;
	case KEY_BACKSPACE:         line_backspace(line);       break;
	case KEY_DELETE:            line_delete(line);          break;
	case MOD_CTRL | KEY_A:      line_ctrl_a(line);          break;
	case MOD_CTRL | KEY_C:      line_ctrl_c(line);          break;
	case MOD_CTRL | KEY_X:      line_ctrl_x(line);          break;
	case MOD_CTRL | KEY_V:      line_ctrl_v(line);          break;
	case KEY_HOME:              line_ctrl_home(line);       break;
	case MOD_SHIFT | KEY_HOME:  line_ctrl_shift_home(line); break;
	case KEY_END:               line_ctrl_end(line);        break;
	case MOD_SHIFT | KEY_END:   line_ctrl_shift_end(line);  break;
	default:
		if(c == '\n')
		{
			key_enter(line);
		}
		else if(isprint(c))
		{
			line_insert(line, c);
		}
		break;
	}

	command_render();
}

/* --- PUBLIC --- */
void shell_init(void)
{
	int i;
	for(i = 0; i < COMMAND_LINES; ++i)
	{
		line_init(&this.Lines[i], _bufs[i], COMMAND_MAX_LENGTH);
	}
}

void shell_open(void)
{
	command_render();
	cls();
	shell_output_reset();
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
			terminal_set(this.X, this.Y, c | this.FG | TERMINAL_BG_BLACK);
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
