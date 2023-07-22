/**
 * @file    panic.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 */

#include "panic.h"
#include "delay.h"
#include "hal.h"

void panic(void)
{
	for(;;)
	{
		GPIOD->ODR = 0xAA;
		GPIOE->ODR = 0xAA;
		delay_ms(1000);
		GPIOD->ODR = 0x55;
		GPIOE->ODR = 0x55;
		delay_ms(1000);
	}
}
