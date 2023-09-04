#include "ps2_ll.h"
#include "ps2.h"
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"

#define PS2_DATA_PORT_IN GPIOG->IDR
#define PS2_DATA_PIN     1

void ps2_ll_init(void)
{
	/* Clock for GPIO Port F */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	/* System conf. clock enable */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/* Routing Pin 0 of Port G -> EXTI0 */
	SYSCFG->EXTICR[0] &= ~(0x0f << (4 * 0));
	SYSCFG->EXTICR[0] |= (0x06 << (4 * 0));

	/* Enable rising and falling trigger */
	EXTI->FTSR |= (1 << 0);
	EXTI->IMR |= (1 << 0);

	NVIC_SetPriorityGrouping(2);
	NVIC_SetPriority(EXTI0_IRQn, 8);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
	EXTI->PR |= (1 << 0);
	ps2_clock_falling_edge();
}

int ps2_data_read(void)
{
	/* Read keyboard data pin */
	return PS2_DATA_PORT_IN & (1 << PS2_DATA_PIN);
}
