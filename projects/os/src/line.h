/**
 * @file    line.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Multiline edit data structure
 */

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

void line_up(Line *line, int tabsize);
void line_shift_up(Line *line, int tabsize);
void line_down(Line *line, int tabsize);
void line_shift_down(Line *line, int tabsize);

void line_left(Line *line);
void line_shift_left(Line *line);
void line_right(Line *line);
void line_shift_right(Line *line);

void line_backspace(Line *line);
void line_delete(Line *line);

void line_ctrl_a(Line *line);
void line_ctrl_c(Line *line);
void line_ctrl_x(Line *line);
void line_ctrl_v(Line *line);

void line_home(Line *line);
void line_shift_home(Line *line);
void line_end(Line *line);
void line_shift_end(Line *line);

void line_ctrl_home(Line *line);
void line_ctrl_shift_home(Line *line);
void line_ctrl_end(Line *line);
void line_ctrl_shift_end(Line *line);

void line_insert(Line *line, int c);

#endif /* __LINE_H__ */
