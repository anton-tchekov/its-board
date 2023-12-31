/**
 * @file    terminal.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Terminal
 */

#ifndef __TERMINAL_H__
#define __TERMINAL_H__

/** Terminal width in characters */
#define TERMINAL_W                  60

/** Terminal height in characters */
#define TERMINAL_H                  20

/* Bit 15 */
#define TERMINAL_BOLD           0x8000

/* Bits 11..8 */
enum TERMINAL_FG
{
	TERMINAL_FG_BLACK          = 0x000,
	TERMINAL_FG_WHITE          = 0x100,
	TERMINAL_FG_RED            = 0x200,
	TERMINAL_FG_GREEN          = 0x300,
	TERMINAL_FG_BLUE           = 0x400,
	TERMINAL_FG_LIGHT_GRAY     = 0x500,
	TERMINAL_FG_MAGENTA        = 0x600,
	TERMINAL_FG_YELLOW         = 0x700,
	TERMINAL_FG_LIGHT_YELLOW   = 0x800,
	TERMINAL_FG_LIGHT_BLUE     = 0x900,
	TERMINAL_FG_LIGHT_GREEN    = 0xA00,
	TERMINAL_FG_ORANGE         = 0xB00,
	TERMINAL_FG_DARK_GREEN     = 0xC00,
	TERMINAL_FG_DARK_BLUE      = 0xD00,
	TERMINAL_FG_RESERVED_8     = 0xE00,
	TERMINAL_FG_RESERVED_9     = 0xF00,
};

/* Bits 14..12 */
enum TERMINAL_BG
{
	TERMINAL_BG_BLACK         = 0x0000,
	TERMINAL_BG_WHITE         = 0x1000,
	TERMINAL_BG_ORANGE        = 0x2000,
	TERMINAL_BG_BLUE          = 0x3000,
	TERMINAL_BG_RESERVED_0    = 0x4000,
	TERMINAL_BG_RESERVED_1    = 0x5000,
	TERMINAL_BG_RESERVED_2    = 0x6000,
	TERMINAL_BG_RESERVED_3    = 0x7000,
};

/**
 * @brief Place a character at the specified position
 *
 * @param x X Position
 * @param y Y-Position
 * @param v Character (bitwise-or of ASCII character and FG/BG enums)
 */
void terminal_set(int x, int y, int v);

/**
 * @brief Clear the terminal with black background and white foreground
 */
void terminal_clear(void);

#endif /* __TERMINAL_H__ */
