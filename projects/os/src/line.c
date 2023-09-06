#include "line.h"
#include "clipboard.h"
#include <string.h>

void line_init(Line *line, char *buffer, int size)
{
	line->Size = size;
	line->Length = 0;
	line->Cursor = 0;
	line->Selection = 0;
	line->Text = buffer;
}

void line_selection_get(Line *line, int *start, int *end)
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
	line_selection_get(line, &start, &end);
	sel_len = end - start;
	new_len = line->Length + len - sel_len;
	if(new_len > line->Size)
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
	line_selection_get(line, &start, &end);
	memmove(line->Text + start, line->Text + end, line->Length - end);
	line->Length -= (end - start);
	line->Cursor = start;
	line->Selection = start;
}

static int min(int a, int b)
{
	return a < b ? a : b;
}

static int max(int a, int b)
{
	return a > b ? a : b;
}

#if 0
static int cur_x_pos(Line *line)
{
	const char *start, *cur, *p;

	start = line->Text;
	cur = start + line->Cursor;
	p = cur;
	while(p > start)
	{
		if(*p == '\n')
		{
			break;
		}

		--p;
	}

	return cur - p;
}

static int next_line_start(Line *line)
{
	int i = line->Cursor;
	int len = line->Length;
	const char *text = line->Text;

	for(; i < len; ++i)
	{
		if(text[i] == '\n')
		{
			++i;
			break;
		}
	}

	line->Cursor = i;
}

static int prev_line_start(Line *line)
{
	int x;
	int i = line->Cursor;
	const char *text = line->Text;

	for(; i >= 0; --i)
	{
		if(text[i] == '\n')
		{
			--i;
			break;
		}
	}

	for(; i >= 0; --i)
	{
		if(text[i] == '\n')
		{
			++i;
			break;
		}
	}

	line->Cursor = i;
}
#endif

/* --- PUBLIC --- */
void line_key_up(Line *line)
{
#if 0
	line->Cursor = min(line->Cursor, line->Selection);
	prev_line_start(line);
	line->Selection = line->Cursor;
#endif
}

void line_key_shift_up(Line *line)
{
#if 0
	prev_line_start(line);
#endif
}

void line_key_down(Line *line)
{
#if 0
	line->Cursor = max(line->Cursor, line->Selection);
	next_line_start(line);
	line->Selection = line->Cursor;
#endif
}

void line_key_shift_down(Line *line)
{
#if 0
	next_line_start(line);
#endif
}

void line_key_left(Line *line)
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
		line->Cursor = min(line->Cursor, line->Selection);
		line->Selection = line->Cursor;
	}
}

void line_key_shift_left(Line *line)
{
	if(line->Cursor > 0)
	{
		--line->Cursor;
	}
}

void line_key_right(Line *line)
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
		line->Cursor = max(line->Cursor, line->Selection);
		line->Selection = line->Cursor;
	}
}

void line_key_shift_right(Line *line)
{
	if(line->Cursor < line->Length)
	{
		++line->Cursor;
	}
}

void line_key_backspace(Line *line)
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

void line_key_delete(Line *line)
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

void line_key_ctrl_a(Line *line)
{
	line->Selection = 0;
	line->Cursor = line->Length;
}

void line_key_ctrl_c(Line *line)
{
	int start, end;
	line_selection_get(line, &start, &end);
	clipboard_save(line->Text + start, end - start);
}

void line_key_ctrl_x(Line *line)
{
	line_key_ctrl_c(line);
	selection_delete(line);
}

void line_key_ctrl_v(Line *line)
{
	int len;
	const char *text = clipboard_get(&len);
	selection_replace(line, text, len);
}

void line_key_home(Line *line)
{
	line->Cursor = 0;
	line->Selection = line->Cursor;
}

void line_key_shift_home(Line *line)
{
	line->Cursor = 0;
}

void line_key_end(Line *line)
{
	line->Cursor = line->Length;
	line->Selection = line->Cursor;
}

void line_key_shift_end(Line *line)
{
	line->Cursor = line->Length;
}

void line_key_insert(Line *line, int c)
{
	if(line->Selection == line->Cursor)
	{
		if(line->Length < line->Size)
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
