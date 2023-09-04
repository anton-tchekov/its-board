#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#define TERMINAL_W                  60
#define TERMINAL_H                  20

#define TERMINAL_COLOR(FG, BG)        ((FG) | ((BG) << 4))
#define TERMINAL_CHAR(C, STYLE)       ((C) | ((STYLE) << 8))

enum TERMINAL_COLOR
{
	TERMINAL_BLACK,
	TERMINAL_RED,
	TERMINAL_GREEN,
	TERMINAL_YELLOW,
	TERMINAL_BLUE,
	TERMINAL_MAGENTA,
	TERMINAL_CYAN,
	TERMINAL_WHITE,

	TERMINAL_BRIGHT_BLACK,
	TERMINAL_BRIGHT_RED,
	TERMINAL_BRIGHT_GREEN,
	TERMINAL_BRIGHT_YELLOW,
	TERMINAL_BRIGHT_BLUE,
	TERMINAL_BRIGHT_MAGENTA,
	TERMINAL_BRIGHT_CYAN,
	TERMINAL_BRIGHT_WHITE,
};

void terminal_set(int x, int y, int v);

#define terminal_print(v)

#endif /* __TERMINAL_H__ */
