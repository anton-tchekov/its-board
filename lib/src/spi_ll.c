/**
 * @file    spi_ll.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-25
 */

#include "spi_ll.h"
#include "stm32f4xx_hal.h"

#define GPIO_MODE_AF      0x02
#define GPIO_SPEED_50MHZ  0x02
#define GPIO_PUPD_DOWN    0x02

void spi_ll_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER |=
		(GPIO_MODE_AF << (5 * 2)) |
		(GPIO_MODE_AF << (6 * 2)) |
		(GPIO_MODE_AF << (7 * 2));

	GPIOA->OSPEEDR |=
		(GPIO_SPEED_50MHZ << (5 * 2)) |
		(GPIO_SPEED_50MHZ << (6 * 2)) |
		(GPIO_SPEED_50MHZ << (7 * 2));

	GPIOA->PUPDR |= (GPIO_PUPD_DOWN << (5 * 2));

	GPIOA->AFR[0] |=
		(GPIO_AF5_SPI1 << (5 * 4)) |
		(GPIO_AF5_SPI1 << (6 * 4)) |
		(GPIO_AF5_SPI1 << (7 * 4));

	
	spi_ll_fast();

}

void spi_ll_fast(void)
{
	SPI1->CR1 &= ~(1 << 6);
	SPI1->CR1 = (1 << 9) | (1 << 8) | (1 << 2) |
		(SPI_BAUDRATEPRESCALER_16 & SPI_CR1_BR_Msk);
	
	SPI1->CR2 = 0;
	SPI1->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;
	SPI1->CR1 |= (1 << 6);
}

void spi_ll_slow(void)
{
	SPI1->CR1 &= ~(1 << 6);
	SPI1->CR1 = (1 << 9) | (1 << 8) | (1 << 2) |
		(SPI_BAUDRATEPRESCALER_256 & SPI_CR1_BR_Msk);
	
	SPI1->CR2 = 0;
	SPI1->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;
	SPI1->CR1 |= (1 << 6);
}

int spi_ll_xchg(int val)
{
	while(!(SPI1->SR & SPI_SR_TXE)) {}
	SPI1->DR = val;
	while(!(SPI1->SR & SPI_SR_RXNE)) {}
	while(SPI1->SR & SPI_SR_BSY) {}
	return SPI1->DR;
}
