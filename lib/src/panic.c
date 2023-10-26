/**
 * @file    panic.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 */

#include "panic.h"
#include "delay.h"
#include "stm32f4xx_hal.h"

void panic(void)
{
	for(;;)
	{
		GPIOD->BSRR = 0xFF << 16;
		GPIOE->BSRR = 0xFF << 16;
		GPIOD->BSRR = 0xAA;
		GPIOE->BSRR = 0xAA;
		delay_ms(1000);
		GPIOD->BSRR = 0xFF << 16;
		GPIOE->BSRR = 0xFF << 16;
		GPIOD->BSRR = 0x55;
		GPIOE->BSRR = 0x55;
		delay_ms(1000);
	}
}
