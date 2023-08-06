/**
 * @file    hal.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 * @brief   Simple HAL because the one from STM is bloat
 */

#ifndef __HAL_H__
#define __HAL_H__

#include <stdint.h>

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
	volatile uint32_t OR;
} TIM_TypeDef;

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t RESERVED2;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	volatile uint32_t RESERVED4;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;
} RCC_TypeDef;

typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
} GPIO_TypeDef;

typedef struct
{
	volatile const uint32_t CPUID;
	volatile uint32_t ICSR;
	volatile uint32_t VTOR;
	volatile uint32_t AIRCR;
	volatile uint32_t SCR;
	volatile uint32_t CCR;
	volatile uint8_t  SHP[12U];
	volatile uint32_t SHCSR;
	volatile uint32_t CFSR;
	volatile uint32_t HFSR;
	volatile uint32_t DFSR;
	volatile uint32_t MMFAR;
	volatile uint32_t BFAR;
	volatile uint32_t AFSR;
	volatile const uint32_t PFR[2U];
	volatile const uint32_t DFR;
	volatile const uint32_t ADR;
	volatile const uint32_t MMFR[4U];
	volatile const uint32_t ISAR[5U];
	uint32_t RESERVED0[5U];
	volatile uint32_t CPACR;
} SCB_Type;

#define PERIPH_BASE           0x40000000
#define APB1PERIPH_BASE       PERIPH_BASE
#define TIM2                  ((TIM_TypeDef *)(APB1PERIPH_BASE + 0x0000UL))

#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x20000UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
#define RCC                   ((RCC_TypeDef *)RCC_BASE)
#define SCB                   ((SCB_Type *)SCB_BASE)

#define RCC_APB1ENR_TIM2EN   1
#define TIM_CR1_CEN          1

#define VECT_TAB_OFFSET       0x00
#define FLASH_BASE            0x08000000UL
#define SCS_BASE              0xE000E000UL
#define SCB_BASE              (SCS_BASE + 0x0D00UL)

#define GPIOA                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0000UL))
#define GPIOB                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0400UL))
#define GPIOC                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0800UL))
#define GPIOD                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x0C00UL))
#define GPIOE                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x1000UL))
#define GPIOF                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x1400UL))
#define GPIOG                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x1800UL))
#define GPIOH                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x1C00UL))
#define GPIOI                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x2000UL))
#define GPIOJ                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x2400UL))
#define GPIOK                 ((GPIO_TypeDef *)(AHB1PERIPH_BASE + 0x2800UL))

#define GPIO_CLOCK_EN(PORT_EN) \
	do { \
		volatile uint32_t tmpreg; \
		RCC->AHB1ENR |= (PORT_EN);\
		tmpreg = (RCC->AHB1ENR & (PORT_EN));\
		(void)tmpreg; \
	} while(0)

#define __HAL_RCC_GPIOD_CLK_ENABLE() GPIO_CLOCK_EN(1 << 3)
#define __HAL_RCC_GPIOE_CLK_ENABLE() GPIO_CLOCK_EN(1 << 4)
#define __HAL_RCC_GPIOF_CLK_ENABLE() GPIO_CLOCK_EN(1 << 5)
#define __HAL_RCC_GPIOG_CLK_ENABLE() GPIO_CLOCK_EN(1 << 6)

#endif /* __HAL_H__ */
