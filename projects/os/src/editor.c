#include "editor.h"
#include "line.h"
#include "terminal.h"
#include "keyboard.h"
#include "util.h"
#include "font.h"

#include <stddef.h>
#include <ctype.h>
#include <string.h>

static char _buf[1024];
static Line line;
static int _page_y, _page_x;
static int _tab_size;
static int _show_space;
static int _syntax;

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

static void page_shift_left(void)
{
	if(_page_x > -5)
	{
		--_page_x;
	}
}

static void page_shift_right(void)
{
	++_page_x;
}

static void page_shift_up(void)
{
	if(_page_y > 0)
	{
		--_page_y;
	}
}

static void page_shift_down(void)
{
	++_page_y;
}

static void toggle_tab_size(void)
{
	_tab_size <<= 1;
	if(_tab_size > 8)
	{
		_tab_size = 2;
	}
}

static void toggle_show_space(void)
{
	_show_space = !_show_space;
}

static void toggle_syntax_color(void)
{
	_syntax = !_syntax;
}

static int _tab(uint16_t *temp, int x, int y, int bg)
{
	int color = TERMINAL_FG_LIGHT_GRAY | bg;
	int n = x & (_tab_size - 1);
	if(_show_space)
	{
		if(n == _tab_size - 1)
		{
			_set(temp, x, y, CHAR_TAB_BOTH | color);
			++x;
		}
		else
		{
			_set(temp, x, y, CHAR_TAB_START | color);
			++x;
			for(++n; n < _tab_size - 1; ++n)
			{
				_set(temp, x, y, CHAR_TAB_MIDDLE | color);
				++x;
			}
			_set(temp, x, y, CHAR_TAB_END | color);
			++x;
		}
	}
	else
	{
		for(; n < _tab_size; ++n)
		{
			_set(temp, x, y, ' ' | color);
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
		_set(temp, x, y, c | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
		n /= 10;
		--x;
	}
	while(n > 0);
}

static void editor_blit(uint16_t *buf)
{
	int i, x, y;

	i = 0;
	for(y = 0; y < TERMINAL_H; ++y)
	{
		for(x = 0; x < TERMINAL_W; ++x, ++i)
		{
			terminal_set(x, y, buf[i]);
		}
	}
}

static int is_identifer_start(int c)
{
	return isalpha(c) || c == '_';
}

static int is_identifier_char(int c)
{
	return isalnum(c) || c == '_';
}

static int is_opening_bracket(int c)
{
	return c == '(' || c == '[' || c == '{';
}

static int is_closing_bracket(int c)
{
	return c == ')' || c == ']' || c == '}';
}

static int syntax_color(int c)
{
	/*
	if(!keyword_remain && !isalnum(text[i - 1]))
	{
		i32 word_len;

		for(j = i; j < len; ++j)
		{
			if(!isalnum(text[j]))
			{
				break;
			}
		}

		if(is_opening_bracket(text[j]))
		{

		}

		word_len = j - i;
		if(keyword_detect(&text[i], word_len))
		{
			keyword_remain = word_len;
			color = COLOR_BLUE;
		}
	}

	if(is_opening_bracket(c))
	{
		color = _get_bracket_color(brackets);
		++brackets;
	}
	else if(is_closing_bracket(c))
	{
		--brackets;
		color = _get_bracket_color(brackets);
	}
	else if(isdigit(c))
	{
		color = COLOR_LIME;
	}

	if(keyword_remain)
	{
		--keyword_remain;
		color = COLOR_BLUE;
	}

	if(c == '\'' && text[i - 1] != '\\' && !in_string)
	{
		color = COLOR_ORANGE;
		in_char = !in_char;
	}

	if(c == '"' && text[i - 1] != '\\' && !in_char)
	{
		color = COLOR_ORANGE;
		in_string = !in_string;
	}

	if(c == '/' && text[i + 1] == '*')
	{
		color = COLOR_GREEN;
		in_comment = true;
	}

	if(c == '/' && text[i - 1] == '*')
	{
		color = COLOR_GREEN;
		in_comment = false;
	}

	if(in_comment)
	{
		color = COLOR_GREEN;
	}
	else if(in_char || in_string)
	{
		color = COLOR_ORANGE;
	}
	else if()

	return color;
	*/

	return 0;
}

static void editor_render(void)
{
	uint16_t temp[TERMINAL_W * TERMINAL_H];
	int i, x, y, c, len, start, end, fg, bg;

	line_selection_get(&line, &start, &end);
	memset(temp, 0, sizeof(temp));
	x = 0;
	y = 0;
	len = line.Length;
	_buf[len++] = '\n';

	for(i = 0; i < len; ++i)
	{
		if(x == 0)
		{
			linenumber(temp, y);
		}

		fg = TERMINAL_FG_WHITE;

		c = _buf[i];
		if(i == line.Cursor)
		{
			fg = TERMINAL_FG_BLACK;
			bg = TERMINAL_BG_WHITE;
		}
		else if(i >= start && i < end)
		{
			bg = TERMINAL_BG_BLUE;
		}
		else
		{
			bg = TERMINAL_BG_BLACK;
		}

		if(c == '\n')
		{
			_set(temp, x, y, ' ' | TERMINAL_FG_WHITE | bg);
			x = 0;
			++y;
		}
		else if(c == '\t')
		{
			x = _tab(temp, x, y, bg);
		}
		else
		{
			if(_show_space && c == ' ')
			{
				c = CHAR_VISIBLE_SPACE;
				fg = TERMINAL_FG_LIGHT_GRAY;
			}

			_set(temp, x, y, c | fg | bg);
			++x;
		}
	}

	editor_blit(temp);
}

void editor_open(const char *filename)
{
	_tab_size = 4;
	_show_space = 1;
	_syntax = 0;
	line_init(&line, _buf, 1024);
	editor_render();
}

void editor_key(int key, int c)
{
	switch(key)
	{
	case KEY_LEFT:              line_left(&line);                  break;
	case MOD_SHIFT | KEY_LEFT:  line_shift_left(&line);            break;
	case MOD_CTRL | KEY_LEFT:   page_shift_left();                 break;
	case KEY_RIGHT:             line_right(&line);                 break;
	case MOD_SHIFT | KEY_RIGHT: line_shift_right(&line);           break;
	case MOD_CTRL | KEY_RIGHT:  page_shift_right();                break;
	case KEY_UP:                line_up(&line, _tab_size);         break;
	case MOD_SHIFT | KEY_UP:    line_shift_up(&line, _tab_size);   break;
	case MOD_CTRL | KEY_UP:     page_shift_up();                   break;
	case KEY_DOWN:              line_down(&line, _tab_size);       break;
	case MOD_SHIFT | KEY_DOWN:  line_shift_down(&line, _tab_size); break;
	case MOD_CTRL | KEY_DOWN:   page_shift_down();                 break;

	case KEY_BACKSPACE:         line_backspace(&line);             break;
	case KEY_DELETE:            line_delete(&line);                break;
	case MOD_CTRL | KEY_A:      line_ctrl_a(&line);                break;
	case MOD_CTRL | KEY_C:      line_ctrl_c(&line);                break;
	case MOD_CTRL | KEY_X:      line_ctrl_x(&line);                break;
	case MOD_CTRL | KEY_V:      line_ctrl_v(&line);                break;
	case KEY_HOME:              line_home(&line);                  break;
	case MOD_SHIFT | KEY_HOME:  line_shift_home(&line);            break;
	case KEY_END:               line_end(&line);                   break;
	case MOD_SHIFT | KEY_END:   line_shift_end(&line);             break;
	case MOD_CTRL | KEY_T:      toggle_tab_size();                 break;
	case MOD_CTRL | KEY_J:      toggle_show_space();               break;
	case MOD_CTRL | KEY_K:      toggle_syntax_color();             break;

	case MOD_CTRL | KEY_HOME:             line_ctrl_home(&line);       break;
	case MOD_CTRL | MOD_SHIFT | KEY_HOME: line_ctrl_shift_home(&line); break;
	case MOD_CTRL | KEY_END:              line_ctrl_end(&line);        break;
	case MOD_CTRL | MOD_SHIFT | KEY_END:  line_ctrl_shift_end(&line);  break;

	default:
		if(c == '\n' || c == '\t' || isprint(c))
		{
			line_insert(&line, c);
		}
		break;
	}

	editor_render();
}
