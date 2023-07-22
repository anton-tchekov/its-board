/**
 * @file    timer.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 */

#include "timer.h"

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
