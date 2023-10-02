/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 * @brief   OS main function
 */

#include "init.h"
#include "timer.h"
#include "lcd.h"
#include "ps2.h"

#include "terminal.h"
#include "mode.h"

#include "login.h"
#include "shell.h"
#include "editor.h"
#include "manager.h"
#include "test.h"

static void os_key(void)
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
	case MOD_OS | KEY_0: mode_set(MODE_LOGIN);   break;
	case MOD_OS | KEY_1: mode_set(MODE_SHELL);   break;
	case MOD_OS | KEY_2: mode_set(MODE_EDITOR);  break;
	case MOD_OS | KEY_3: mode_set(MODE_MANAGER); break;
	case MOD_OS | KEY_7: mode_set(MODE_TEST);    break;
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

	login_init();
	shell_init();
	editor_init();
	manager_init();

	for(;;)
	{
		os_key();
	}

	return 0;
}
