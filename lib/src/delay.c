/**
 * @file    delay.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 */

#include "delay.h"
#include "timer.h"

void delay_us(uint32_t us)
{
	uint32_t ts_end = timer_get() + us * TICKS_PER_US;
	while(timer_get() < ts_end) {}
}

void delay_ms(uint32_t ms)
{
	delay_us(1000 * ms);
}
