/**
 * @file    editor.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "editor.h"
#include "line.h"
#include "terminal.h"
#include "keyboard.h"
#include "util.h"
#include "font.h"
#include "mode.h"
#include "overlay.h"
#include "ctype_ext.h"
#include "nanoc_lexer_identifier.h"
#include "ff.h"
#include "ffstatus.h"

#include <stddef.h>
#include <string.h>

static char _buf[1024];
static Line line;
static int _page_y, _page_x;
static int _tab_size;
static int _show_space;
static int _syntax;
static char _filename[60];

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

static int syntax_color(const char *line, int len, int i, int *fg)
{
	int c = line[i];
	int next = (i + 1 < len) ? line[i + 1] : '\0';
	int color = TERMINAL_FG_WHITE;

	if(c == '/' && next == '/')
	{
		color = TERMINAL_FG_DARK_GREEN;
		for(; i < len && line[i] != '\n'; ++i) {}
	}
	else if(c == '/' && next == '*')
	{
		color = TERMINAL_FG_DARK_GREEN;
		for(i += 2; i < len; ++i)
		{
			if(line[i] == '*' && line[i + 1] == '/')
			{
				i += 2;
				break;
			}
		}
	}
	else if(c == '\"' || c == '\'')
	{
		int save = c;
		int esc = 0;

		color = TERMINAL_FG_ORANGE;
		for(++i; i < len; ++i)
		{
			c = line[i];
			if(esc)
			{
				esc = 0;
			}
			else if(c == '\\')
			{
				esc = 1;
			}
			else if(c == save)
			{
				break;
			}
		}

		++i;
	}
	else if(c == '(' || c == ')')
	{
		color = TERMINAL_FG_BLUE;
		++i;
	}
	else if(c == '[' || c == ']')
	{
		color = TERMINAL_FG_MAGENTA;
		++i;
	}
	else if(c == '{' || c == '}')
	{
		color = TERMINAL_FG_YELLOW;
		++i;
	}
	else if(is_identifer_start(c))
	{
		NanoC_TokenType tt;
		int start = i;
		for(; i < len && is_identifier_char(c = line[i]); ++i) {}

		tt = nanoc_keyword_detect(line + start, i - start);
		if(tt == NANOC_TT_INT)
		{
			color = TERMINAL_FG_DARK_BLUE;
		}
		else if(tt != NANOC_TT_IDENTIFIER)
		{
			color = TERMINAL_FG_MAGENTA;
		}
		else if(c == '(')
		{
			color = TERMINAL_FG_LIGHT_YELLOW;
		}
		else if(c == '[')
		{
			color = TERMINAL_FG_LIGHT_BLUE;
		}
	}
	else if(c == '0')
	{
		color = TERMINAL_FG_LIGHT_GREEN;
		++i;
		c = line[i];
		if(c == 'x' || c == 'X')
		{
			/* Hex */
			for(++i; i < len && isxdigit(line[i]); ++i) {}
		}
		else if(c == 'b' || c == 'B')
		{
			/* Binary */
			for(++i; i < len && is_binary(line[i]); ++i) {}
		}
		else
		{
			/* Octal */
			for(; i < len && is_octal(line[i]); ++i) {}
		}
	}
	else if(c >= '1' && c <= '9')
	{
		/* Decimal */
		color = TERMINAL_FG_LIGHT_GREEN;
		for(; i < len && isdigit(line[i]); ++i) {}
	}
	else
	{
		++i;
	}

	*fg = color;
	return i;
}

static void editor_render(void)
{
	uint16_t temp[TERMINAL_W * TERMINAL_H];
	int i, x, y, c, len, start, end, fg, bg;
	int syntax_fg, syntax_end;

	line_selection_get(&line, &start, &end);
	memset(temp, 0, sizeof(temp));
	x = 0;
	y = 0;
	syntax_fg = 0;
	syntax_end = 0;
	len = line.Length;
	_buf[len++] = '\n';

	for(i = 0; i < len; ++i)
	{
		if(x == 0)
		{
			linenumber(temp, y);
		}

		if(_syntax)
		{
			if(i >= syntax_end)
			{
				syntax_end = syntax_color(_buf, len, i, &syntax_fg);
			}

			fg = syntax_fg;
		}
		else
		{
			fg = TERMINAL_FG_WHITE;
		}

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

void editor_open(void)
{
	editor_render();
}

void editor_init(void)
{
	_page_x = -5;
	_tab_size = 4;
	_show_space = 1;
	_syntax = 0;
	line_init(&line, _buf, sizeof(_buf));
}

static void reset(void)
{
	mode_set(MODE_EDITOR);
}

static void editor_save_callback(int yes, char *path)
{
	FIL fp;
	unsigned int written;
	int result;

	if(!yes)
	{
		reset();
		return;
	}

	result = f_open(&fp, path, FA_CREATE_ALWAYS | FA_WRITE);
	if(result)
	{
		alert(OVERLAY_ERROR, reset,
			"Failed to open file for writing:\n"
			"  %s\n"
			"%s", path, f_status_str(result));
		return;
	}

	result = f_write(&fp, line.Text, line.Length, &written);
	if(result)
	{
		f_close(&fp);
		alert(OVERLAY_ERROR, reset,
			"Failed to write text buffer to file:\n"
			"  %s\n"
			"%s", path, f_status_str(result));
		return;
	}

	f_close(&fp);
	reset();
}

void editor_save(void)
{
	prompt(OVERLAY_NORMAL, editor_save_callback,
		"Save As:", _filename);
}

static int check_text(const char *str, size_t len)
{
	const char *end = str + len;
	while(str < end)
	{
		int c = *str++;
		if(!isprint(c) && c != '\t' && c != '\n')
		{
			return 1;
		}
	}

	return 0;
}

static int iscfile(const char *path)
{
	size_t len = strlen(path);
	return len > 2 && path[len - 1] == 'C' && path[len - 2] == '.';
}

void editor_load(const char *path)
{
	FIL fp;
	unsigned int read = 0;
	int result;

	result = f_open(&fp, path, FA_READ);
	if(result)
	{
		alert(OVERLAY_ERROR, reset,
			"Failed to open file for reading:\n"
			"  %s\n"
			"%s", path, f_status_str(result));
		return;
	}

	result = f_read(&fp, _buf, sizeof(_buf), &read);
	if(result)
	{
		alert(OVERLAY_ERROR, reset,
			"Failed to read file contents:\n"
			"  %s\n"
			"%s", path, f_status_str(result));
		f_close(&fp);
		return;
	}

	f_close(&fp);
	if(check_text(_buf, read))
	{
		alert(OVERLAY_ERROR, reset,
			"Editor cannot open binary file");
		return;
	}

	strcpy(_filename, path);
	_syntax = iscfile(path);
	editor_open();
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

	case MOD_CTRL | KEY_S:      editor_save();                     break;

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
