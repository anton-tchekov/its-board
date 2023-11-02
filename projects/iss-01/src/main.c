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

static uint32_t nsample(uint32_t count)
{
	uint32_t value, i;
	value = 0;
	for(i = 0; i < count; ++i)
	{
		value += adc_read(2, 6);
	}

	return value / count;
}

static void adc_readloop(void)
{
	char buf[128];
	int32_t value, distance;
	for(;;)
	{
		value = nsample(NUM_SAMPLES);
		distance = gp2_get_distance(value);
		sprintf(buf, "%6d | %6d mm", value, distance);
		uart_tx_str(buf);
		uart_tx('\n');
		font_str(10, 10, buf, COLOR_WHITE, COLOR_BLACK, Terminus16_Bold);
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
