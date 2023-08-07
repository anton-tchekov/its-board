/**
 * @file    timer.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 */

#include "timer.h"
#include <stdio.h>
#include <inttypes.h>

void timer_init(void)
{
	/* Enable Timer Clock */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	/* Disable Timer */
	TIM2->CR1 = 0;
	TIM2->CR2 = 0;

	/* No Prescaler (90 MHz) */
	TIM2->PSC = 0;

	/* Auto Reload Register */
	TIM2->ARR = 0xffffffff;

	/* Disable Interrupt */
	TIM2->DIER  = 0;

	/* Enable Timer */
	TIM2->CR1 = TIM_CR1_CEN;
}

#if 0

void timer_delta_str(char *str, uint32_t dt)
{
	uint32_t s, ms, us, ns;

	s = dt / TICKS_PER_S;
	dt -= s * TICKS_PER_S;

	ms = dt / TICKS_PER_MS;
	dt -= ms * TICKS_PER_MS;

	us = dt / TICKS_PER_US;
	dt -= us * TICKS_PER_US;

	ns = 1000 * dt / 90;

	if(s)
	{
		sprintf(str, "%"PRIu32",%3"PRIu32" s", s, ms);
	}
	else if(ms)
	{
		sprintf(str, "%"PRIu32",%3"PRIu32" ms", ms, us);
	}
	else if(us)
	{
		sprintf(str, "%"PRIu32",%3"PRIu32" us", us, ns);
	}
	else
	{
		sprintf(str, "%3"PRIu32" ns", ns);
	}
}

#endif
