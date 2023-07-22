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

#define PIN 4

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	its_board_init();
	timer_init();

	for(;;)
	{
		GPIOD->BSRR = (1 << PIN);
		delay_ms(1000);
		GPIOD->BSRR = (1 << (PIN + 16));
		delay_ms(1000);
	}

	return 0;
}
