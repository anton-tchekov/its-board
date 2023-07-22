/**
 * @file    init.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 * @brief   Board initialization functions
 */

#include "init.h"
#include "hal.h"

void its_board_init(void)
{
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
	RCC->CR |= 0x00000001;
	RCC->CFGR = 0x00000000;
	RCC->CR &= 0xFEF6FFFF;
	RCC->PLLCFGR = 0x24003010;
	RCC->CR &= 0xFFFBFFFF;
	RCC->CIR = 0x00000000;
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;

#if 0
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_InitTick(TICK_INT_PRIORITY);
	HAL_MspInit();
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { panic(); }
	if(HAL_PWREx_EnableOverDrive() != HAL_OK) { panic(); }
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
		RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) { panic(); }
#endif

	/* Ports D, E, F, G */
	GPIO_CLOCK_EN(1 << 3);
	GPIO_CLOCK_EN(1 << 4);
	GPIO_CLOCK_EN(1 << 5);
	GPIO_CLOCK_EN(1 << 6);

	/* Ports D, E Output */
	GPIOD->MODER = 0x5555;
	GPIOE->MODER = 0x5555;

	/* All Pullup */
	GPIOD->PUPDR = 0x5555;
	GPIOE->PUPDR = 0x5555;
	GPIOF->PUPDR = 0x5555;
	GPIOG->PUPDR = 0x0555;

	/* All Fastest Speed */
	GPIOD->OSPEEDR = 0xFFFF;
	GPIOE->OSPEEDR = 0xFFFF;
	GPIOF->OSPEEDR = 0xFFFF;
	GPIOG->OSPEEDR = 0x0FFF;
}
