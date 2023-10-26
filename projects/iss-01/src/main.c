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
#include "fonts/Terminus16_Bold.h"
#include "adc.h"
#include <stdio.h>

static void adc_readloop(void)
{
	char buf[128];
	int32_t value;

	for(;;)
	{
		value = adc_read(2, 6);
		sprintf(buf, "%6d", value);
		font_str(10, 10, buf, COLOR_WHITE, COLOR_BLACK, Terminus16_Bold);
		delay_ms(100);
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

	adc_init(2);
	adc_readloop();

	for(;;)
	{
	}

	return 0;
}
