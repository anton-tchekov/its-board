/**
 * @file    adc.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 */

#include "stm32f4xx_hal.h"
#include "assert.h"

#define CR_CLEAR_MASK                     0xFFFC30E0
#define ADC_TWO_SAMPLING_DELAY_8_CYCLES   0x00000300
#define ADC_PRESCALER_DIV4                0x00010000
#define CR1_CLEAR_MASK                    0xF73F0000
#define ADC_CR1_SCANMODE_ENABLE           0x00000100
#define CR2_CLEAR_MASK                    0x8080F0FC
#define SMPR_CLEAR_MASK                   0x00000007
#define ADC_SAMPLE_TIME_56_CYCLES         0x11

static ADC_TypeDef *const adcs[] = { ADC1, ADC2, ADC3 };

void adc_init(uint32_t adc)
{
	static const uint32_t en[] =
	{
		RCC_APB2ENR_ADC1EN,
		RCC_APB2ENR_ADC2EN,
		RCC_APB2ENR_ADC3EN
	};

	uint32_t i;
	ADC_TypeDef *a;

	assert(adc <= 2);
	a = adcs[adc];

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= en[adc];

	i = GPIOF->MODER;
	i = (i & ~(3 << (2 * 8))) | (GPIO_MODE_ANALOG << 2 * 8);
	i = (i & ~(3 << (2 * 9))) | (GPIO_MODE_ANALOG << 2 * 9);
	i = (i & ~(3 << (2 * 10))) | (GPIO_MODE_ANALOG << 2 * 10);
	GPIOF->MODER = i;

	i = GPIOB->MODER;
	i = (i & ~(3 << (2 * 1))) | (GPIO_MODE_ANALOG << 2 * 1);
	GPIOB->MODER = i;

	i = GPIOC->MODER;
	i = (i & ~(3 << (2 * 0))) | (GPIO_MODE_ANALOG << 2 * 0);
	i = (i & ~(3 << (2 * 2))) | (GPIO_MODE_ANALOG << 2 * 2);
	i = (i & ~(3 << (2 * 3))) | (GPIO_MODE_ANALOG << 2 * 3);
	GPIOC->MODER = i;

	i = ADC->CCR;
	i &= CR_CLEAR_MASK;
	i |= ADC_TWO_SAMPLING_DELAY_8_CYCLES | ADC_PRESCALER_DIV4;
	ADC->CCR = i;

	i = a->CR1;
	i &= CR1_CLEAR_MASK;
	i |= ADC_CR1_SCANMODE_ENABLE;
	a->CR1 = i;

	i = a->CR2;
	i &= CR2_CLEAR_MASK;
	i |= ADC_CR2_ADON;
	a->CR2 = i;

	a->SQR1 = 0;
	a->SQR2 = 0;
	a->SQR3 = 0;
}

int32_t adc_read(uint32_t adc, uint32_t channel)
{
	uint32_t i;
	ADC_TypeDef *a;

	assert(adc <= 2);
	assert(channel <= 15);
	a = adcs[adc];

	if(channel < 10)
	{
		i = a->SMPR2;
		i &= ~(SMPR_CLEAR_MASK << (3 * channel));
		i |= ADC_SAMPLE_TIME_56_CYCLES << (3 * channel);
		a->SMPR2 = i;
	}
	else
	{
		i = a->SMPR1;
		i &= ~(SMPR_CLEAR_MASK << (3 * (channel - 10)));
		i |= ADC_SAMPLE_TIME_56_CYCLES << (3 * (channel - 10));
		a->SMPR1 = i;
	}

	a->SQR3 = channel;

	i = 0xFFF;
	a->CR2 |= ADC_CR2_SWSTART;
	while(!(a->SR & ADC_SR_EOC))
	{
		if(--i == 0)
		{
			return -1;
		}
	}

	return a->DR;
}
