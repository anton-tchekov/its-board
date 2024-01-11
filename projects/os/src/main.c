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

#include "shell.h"
#include "editor.h"

#include "thread.h"

#include <stdlib.h>
#include <stdio.h>

static void os_key(void)
{
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
	case MOD_OS | KEY_1: mode_set(MODE_SHELL);   break;
	case MOD_OS | KEY_2: mode_set(MODE_EDITOR);  break;
	default:
		mode_key(event.Key, event.Codepoint);
		break;
	}
}
	char buf[128];

void thread2(void)
{
	int i;
	uint32_t sp = getsp();
	sprintf(buf, "Cur2 SP: %08x\n", sp);
	shell_print(buf);

	for(i = 0; i < 5; ++i)
	{
		shell_print("Message from second thread");
		yield();
	}
}

void thread1(void)
{
	int i;

	shell_print("Init 2!\n");
	thread_init(getsp() - 64 * 1024, thread2);

	for(i = 0; i < 5; ++i)
	{
		shell_print("Message from first thread");
		yield();
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
	lcd_init(COLOR_BLACK);
	ps2_init();
	srand(314159265);

	shell_init();
	editor_init();

	shell_open();

	for(;;)
	{
		os_key();
	}

	return 0;
}
