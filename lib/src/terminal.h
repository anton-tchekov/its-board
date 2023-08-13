#ifndef __TERMINAL_H__
#define __TERMINAL_H__

enum
{
	ID_BLACK,
	ID_RED,
	ID_GREEN,
	ID_YELLOW,
	ID_BLUE,
	ID_MAGENTA,
	ID_CYAN,
	ID_WHITE,

	ID_BRIGHT_BLACK,
	ID_BRIGHT_RED,
	ID_BRIGHT_GREEN,
	ID_BRIGHT_YELLOW,
	ID_BRIGHT_BLUE,
	ID_BRIGHT_MAGENTA,
	ID_BRIGHT_CYAN,
	ID_BRIGHT_WHITE,
};

void terminal_clear(void);
void terminal_init(void);
void terminal_print(const char *s);
void terminal_char(int c);
void terminal_fg(int color);
void terminal_xy(int x, int y);

#endif /* __TERMINAL_H__ */
