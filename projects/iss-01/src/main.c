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
#include "uart.h"
#include "GP2Y0A21.h"
#include <stdio.h>

#define NUM_SAMPLES 4096

static void adc_readloop(void)
{
	char buf[128];
	int32_t value, distance;
	uint32_t i;

	for(;;)
	{
		value = 0;
		for(i = 0; i < NUM_SAMPLES; ++i)
		{
			value += adc_read(2, 6);
		}

		value /= NUM_SAMPLES;
		distance = gp2_get_distance(value);

		uart_tx_str("Hello World!\n");

		sprintf(buf, "%6d | %6d mm", value, distance);
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
	uart_init(115200);

	adc_init(2);
	adc_readloop();

	for(;;)
	{
	}

	return 0;
}
