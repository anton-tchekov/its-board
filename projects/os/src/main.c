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
#include "login.h"
#include "editor.h"
#include "manager.h"
#include "mode.h"

static int _mode = MODE_LOGIN;

void mode_set(int mode)
{
	_mode = mode;
	switch(_mode)
	{
	case MODE_LOGIN:
		login_open();
		break;

	case MODE_SHELL:
		shell_open();
		break;

	case MODE_EDITOR:
		editor_open();
		break;

	case MODE_MANAGER:
		manager_open();
		break;
	}
}

static void mode_key(int key, int c)
{
	switch(_mode)
	{
	case MODE_LOGIN:
		login_key(key, c);
		break;

	case MODE_SHELL:
		shell_key(key, c);
		break;

	case MODE_EDITOR:
		editor_key(key, c);
		break;

	case MODE_MANAGER:
		manager_key(key, c);
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
	case MOD_OS | KEY_0:
		mode_set(MODE_LOGIN);
		break;

	case MOD_OS | KEY_1:
		mode_set(MODE_SHELL);
		break;

	case MOD_OS | KEY_2:
		mode_set(MODE_EDITOR);
		break;

	case MOD_OS | KEY_3:
		mode_set(MODE_MANAGER);
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

	login_init();
	shell_init();
	editor_init();
	manager_init();

	for(;;)
	{
		os_update();
	}

	return 0;
}
