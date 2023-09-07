#include "line.h"
#include "clipboard.h"
#include <string.h>
#include <ctype.h>

void line_init(Line *line, char *buffer, int size)
{
	line->Size = size;
	line->Length = 0;
	line->SaveX = -1;
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

static int set_x_pos(const char *s, int pos, int target, int len)
{
	int x = 0;
	while(pos < len && x < target)
	{
		int c = s[pos];
		if(c == '\n')
		{
			break;
		}
		if(c == '\t')
		{
			x += 4 - (x & 3);
		}
		else
		{
			++x;
		}

		++pos;
	}

	return pos;
}

static int get_x_pos(const char *s, int pos, int cur)
{
	int x = 0;
	while(pos < cur)
	{
		int c = s[pos];
		if(c == '\t')
		{
			x += 4 - (x & 3);
		}
		else
		{
			++x;
		}

		++pos;
	}

	return x;
}

static int cur_line_start(const char *s, int pos)
{
	while(pos > 0)
	{
		--pos;
		if(s[pos] == '\n')
		{
			return pos + 1;
		}
	}

	return pos;
}

static int cur_line_home(const char *s, int pos)
{
	int start = cur_line_start(s, pos);
	int i = start;
	int c;
	while(((c = s[i]) == ' ') || (c == '\t')) { ++i; }
	return (pos == i) ? start : i;
}

static int cur_line_end(const char *s, int pos, int len)
{
	while(pos < len)
	{
		if(s[pos] == '\n')
		{
			break;
		}

		++pos;
	}

	return pos;
}

static int next_line_start(const char *s, int pos, int len)
{
	while(pos < len)
	{
		if(s[pos] == '\n')
		{
			++pos;
			break;
		}

		++pos;
	}

	return pos;
}

static int get_target(Line *line, int pos, int cur)
{
	if(line->SaveX < 0)
	{
		line->SaveX = get_x_pos(line->Text, pos, cur);
	}

	return line->SaveX;
}

static void next_line_pos(Line *line, int pos)
{
	int cls = cur_line_start(line->Text, pos);
	int nls = next_line_start(line->Text, pos, line->Length);
	int target = get_target(line, cls, pos);
	if(nls == line->Length)
	{
		line->Cursor = line->Length;
		line->SaveX = -1;
	}
	else
	{
		line->Cursor = set_x_pos(line->Text, nls, target, line->Length);
	}
}

static void prev_line_pos(Line *line, int pos)
{
	int cls = cur_line_start(line->Text, pos);
	int pls = (cls > 0) ? cur_line_start(line->Text, cls - 1) : 0;
	int target = get_target(line, cls, pos);
	if(cls == 0)
	{
		line->Cursor = 0;
		line->SaveX = -1;
	}
	else
	{
		line->Cursor = set_x_pos(line->Text, pls, target, line->Length);
	}
}

/* --- PUBLIC --- */
void line_key_up(Line *line)
{
	prev_line_pos(line, min(line->Cursor, line->Selection));
	line->Selection = line->Cursor;
}

void line_key_shift_up(Line *line)
{
	prev_line_pos(line, line->Cursor);
}

void line_key_down(Line *line)
{
	next_line_pos(line, max(line->Cursor, line->Selection));
	line->Selection = line->Cursor;
}

void line_key_shift_down(Line *line)
{
	next_line_pos(line, line->Cursor);
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

	line->SaveX = -1;
}

void line_key_shift_left(Line *line)
{
	if(line->Cursor > 0)
	{
		--line->Cursor;
	}

	line->SaveX = -1;
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

	line->SaveX = -1;
}

void line_key_shift_right(Line *line)
{
	if(line->Cursor < line->Length)
	{
		++line->Cursor;
	}

	line->SaveX = -1;
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

	line->SaveX = -1;
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

	line->SaveX = -1;
}

void line_key_ctrl_a(Line *line)
{
	line->Selection = 0;
	line->Cursor = line->Length;
	line->SaveX = -1;
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
	line->SaveX = -1;
}

void line_key_ctrl_v(Line *line)
{
	int len;
	const char *text = clipboard_get(&len);
	selection_replace(line, text, len);
	line->SaveX = -1;
}

void line_key_home(Line *line)
{
	line->Cursor = cur_line_home(line->Text, line->Cursor);
	line->Selection = line->Cursor;
	line->SaveX = -1;
}

void line_key_shift_home(Line *line)
{
	line->Cursor = cur_line_home(line->Text, line->Cursor);
	line->SaveX = -1;
}

void line_key_end(Line *line)
{
	line->Cursor = cur_line_end(line->Text, line->Cursor, line->Length);
	line->Selection = line->Cursor;
	line->SaveX = -1;
}

void line_key_shift_end(Line *line)
{
	line->Cursor = cur_line_end(line->Text, line->Cursor, line->Length);
	line->SaveX = -1;
}

void line_key_ctrl_home(Line *line)
{
	line->Cursor = 0;
	line->Selection = line->Cursor;
	line->SaveX = -1;
}

void line_key_ctrl_shift_home(Line *line)
{
	line->Cursor = 0;
	line->SaveX = -1;
}

void line_key_ctrl_end(Line *line)
{
	line->Cursor = line->Length;
	line->Selection = line->Cursor;
	line->SaveX = -1;
}

void line_key_ctrl_shift_end(Line *line)
{
	line->Cursor = line->Length;
	line->SaveX = -1;
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

	line->SaveX = -1;
}
