/**
 * @file    uart.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-11-01
 */

#include "uart.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define APB1_FREQUENCY 45000000

#define SETBITS(R, CLEARMASK, SETMASK) \
	(R) = ((R) & ~(CLEARMASK)) | (SETMASK)

static inline void gpio_init(
	GPIO_TypeDef *gpio, uint32_t pin, uint32_t mode, uint32_t type,
	uint32_t speed, uint32_t pull, uint32_t af)
{
	SETBITS(gpio->OTYPER, 1UL << pin, type << pin);
	SETBITS(gpio->OSPEEDR, 3UL << (pin * 2), speed << (pin * 2));
	SETBITS(gpio->PUPDR, 3UL << (pin * 2), pull << (pin * 2));
	SETBITS(gpio->AFR[pin >> 3], 15UL << ((pin & 7) * 4), af << ((pin & 7) * 4));
	SETBITS(gpio->MODER, 3UL << (pin * 2), mode << (pin * 2));
}

void uart_init(uint32_t baud)
{
	RCC->APB1ENR |= (1 << 18) | (1 << 3);
	gpio_init(GPIOD, 8, 2, 0, GPIO_SPEED_HIGH, 0, 7);
	gpio_init(GPIOD, 9, 2, 0, GPIO_SPEED_HIGH, 0, 7);
	USART3->CR1 = 0;
	USART3->BRR = APB1_FREQUENCY / baud;
	USART3->CR1 |= (1 << 13) | (1 << 2) | (1 << 3);
}

void uart_tx(int val)
{
	USART3->DR = val;
	while(!(USART3->SR & USART_SR_TXE)) {}
}

void uart_tx_str(const char *s)
{
	int c;
	while((c = *s++))
	{
		uart_tx(c);
	}
}
