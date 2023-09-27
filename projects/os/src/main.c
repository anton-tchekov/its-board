/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 * @brief   Template Project
 */

#include "init.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "font.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"
#include "ps2.h"
#include "terminal.h"
#include "shell.h"
#include "editor.h"

enum
{
	MODE_SHELL,
	MODE_EDITOR,
};

static int mode = MODE_SHELL;

static void mode_key(int key, int c)
{
	switch(mode)
	{
		case MODE_SHELL:
			shell_key(key, c);
			break;

		case MODE_EDITOR:
			editor_key(key, c);
			break;
	}
}

static void os_update(void)
{
	/* TODO: Replace fake multitasking with
		real multitasking and remove polling */
	KeyEvent event;
	if(!ps2_read(&event))
	{
		return;
	}

	if(event.State == KEYSTATE_RELEASED)
	{
		return;
	}

	switch(event.Key)
	{
		case MOD_OS | KEY_1:
			mode = MODE_SHELL;
			shell_open();
			break;

		case MOD_OS | KEY_2:
			mode = MODE_EDITOR;
			editor_open();
			break;

		default:
			mode_key(event.Key, event.Codepoint);
			break;
	}
}

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	its_board_init();
	timer_init();
	lcd_init(D2U_L2R, COLOR_BLACK);
	ps2_init();

	shell_init();
	editor_init();

	shell_open();
	for(;;)
	{
		os_update();
	}

	return 0;
}
