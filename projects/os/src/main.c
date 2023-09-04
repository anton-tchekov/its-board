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
#include "stdio.h"
#include "terminal.h"
#include "shell.h"

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	its_board_init();
	timer_init();
	lcd_init(D2U_L2R, 1000, COLOR_BLACK);
	ps2_init();

	shell_init();
	for(;;)
	{
		shell_update();
	}

	return 0;
}
