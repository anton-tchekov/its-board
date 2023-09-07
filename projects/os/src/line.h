#ifndef __LINE_H__
#define __LINE_H__

typedef struct
{
	int Size;
	int Length;
	int SaveX;
	int Cursor;
	int Selection;
	char *Text;
} Line;

void line_init(Line *line, char *buffer, int size);

void line_selection_get(Line *line, int *start, int *end);

void line_key_up(Line *line);
void line_key_shift_up(Line *line);
void line_key_down(Line *line);
void line_key_shift_down(Line *line);

void line_key_left(Line *line);
void line_key_shift_left(Line *line);
void line_key_right(Line *line);
void line_key_shift_right(Line *line);

void line_key_backspace(Line *line);
void line_key_delete(Line *line);

void line_key_ctrl_a(Line *line);
void line_key_ctrl_c(Line *line);
void line_key_ctrl_x(Line *line);
void line_key_ctrl_v(Line *line);

void line_key_home(Line *line);
void line_key_shift_home(Line *line);
void line_key_end(Line *line);
void line_key_shift_end(Line *line);

void line_key_ctrl_home(Line *line);
void line_key_ctrl_shift_home(Line *line);
void line_key_ctrl_end(Line *line);
void line_key_ctrl_shift_end(Line *line);

void line_key_insert(Line *line, int c);

#endif /* __LINE_H__ */
