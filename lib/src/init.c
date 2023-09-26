/**
 * @file    init.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 * @brief   Board initialization functions
 */

#include "init.h"
#include "spi_ll.h"
#include "stm32f4xx_hal.h"
#include "panic.h"

#define VECT_TAB_OFFSET              0x00
#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000U

static void _HAL_PWREx_EnableOverDrive(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_OVERDRIVE_ENABLE();
	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ODRDY)) {}
	__HAL_PWR_OVERDRIVESWITCHING_ENABLE();
	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ODSWRDY)) {}
}

static HAL_StatusTypeDef _HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency)
{
	if(FLatency > __HAL_FLASH_GET_LATENCY())
	{
		__HAL_FLASH_SET_LATENCY(FLatency);
		if(__HAL_FLASH_GET_LATENCY() != FLatency)
		{
			return HAL_ERROR;
		}
	}

	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
	{
		if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
		{
			MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);
		}

		if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
		{
			MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV16 << 3));
		}

		MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
	}

	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
	{
		if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
		{
			if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
			{
				return HAL_ERROR;
			}
		}
		else if((RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK) ||
			(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLRCLK))
		{
			if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
			{
				return HAL_ERROR;
			}
		}
		else
		{
			if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
			{
				return HAL_ERROR;
			}
		}

		__HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);
		while(__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))
		{
		}
	}

	if(FLatency < __HAL_FLASH_GET_LATENCY())
	{
		__HAL_FLASH_SET_LATENCY(FLatency);
		if(__HAL_FLASH_GET_LATENCY() != FLatency)
		{
			return HAL_ERROR;
		}
	}

	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
	{
		MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);
	}

	if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
	{
		MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3U));
	}

	return HAL_OK;
}

static HAL_StatusTypeDef _HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct)
{
	uint32_t pll_config;

	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)
	{
		if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSE) ||\
			((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSE)))
		{
			if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET) && (RCC_OscInitStruct->HSEState == RCC_HSE_OFF))
			{
				return HAL_ERROR;
			}
		}
		else
		{
			__HAL_RCC_HSE_CONFIG(RCC_OscInitStruct->HSEState);
			if((RCC_OscInitStruct->HSEState) != RCC_HSE_OFF)
			{
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET) {}
			}
			else
			{
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET) {}
			}
		}
	}

	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)
	{
		if((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_HSI) ||\
			((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_PLL) && ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))
		{
			if((__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) && (RCC_OscInitStruct->HSIState != RCC_HSI_ON))
			{
				return HAL_ERROR;
			}
			else
			{
				__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
			}
		}
		else
		{
			if((RCC_OscInitStruct->HSIState)!= RCC_HSI_OFF)
			{
				__HAL_RCC_HSI_ENABLE();
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET) {}
				__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
			}
			else
			{
				__HAL_RCC_HSI_DISABLE();
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) {}
			}
		}
	}

	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI)
	{
		if((RCC_OscInitStruct->LSIState)!= RCC_LSI_OFF)
		{
			__HAL_RCC_LSI_ENABLE();
			while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET) {}
		}
		else
		{
			__HAL_RCC_LSI_DISABLE();
			while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET) {}
		}
	}
	if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE)
	{
		FlagStatus pwrclkchanged = RESET;

		if(__HAL_RCC_PWR_IS_CLK_DISABLED())
		{
			__HAL_RCC_PWR_CLK_ENABLE();
			pwrclkchanged = SET;
		}

		if(HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
		{
			SET_BIT(PWR->CR, PWR_CR_DBP);
			while(HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP)) {}
		}

		__HAL_RCC_LSE_CONFIG(RCC_OscInitStruct->LSEState);
		if((RCC_OscInitStruct->LSEState) != RCC_LSE_OFF)
		{
			while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET) {}
		}
		else
		{
			while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET) {}
		}

		if(pwrclkchanged == SET)
		{
			__HAL_RCC_PWR_CLK_DISABLE();
		}
	}
	if ((RCC_OscInitStruct->PLL.PLLState) != RCC_PLL_NONE)
	{
		if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)
		{
			if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_ON)
			{
				__HAL_RCC_PLL_DISABLE();
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET) {}
				WRITE_REG(RCC->PLLCFGR, (RCC_OscInitStruct->PLL.PLLSource | \
					RCC_OscInitStruct->PLL.PLLM | \
					(RCC_OscInitStruct->PLL.PLLN << RCC_PLLCFGR_PLLN_Pos) | \
					(((RCC_OscInitStruct->PLL.PLLP >> 1U) - 1U) << RCC_PLLCFGR_PLLP_Pos) | \
					(RCC_OscInitStruct->PLL.PLLQ << RCC_PLLCFGR_PLLQ_Pos)));

				__HAL_RCC_PLL_ENABLE();
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET) {}
			}
			else
			{
				__HAL_RCC_PLL_DISABLE();
				while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET) {}
			}
		}
		else
		{
			if((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF)
			{
				return HAL_ERROR;
			}
			else
			{
				pll_config = RCC->PLLCFGR;
				if((READ_BIT(pll_config, RCC_PLLCFGR_PLLSRC) != RCC_OscInitStruct->PLL.PLLSource) ||
					(READ_BIT(pll_config, RCC_PLLCFGR_PLLM) != RCC_OscInitStruct->PLL.PLLM) ||
					(READ_BIT(pll_config, RCC_PLLCFGR_PLLN) != RCC_OscInitStruct->PLL.PLLN) ||
					(READ_BIT(pll_config, RCC_PLLCFGR_PLLP) != RCC_OscInitStruct->PLL.PLLP) ||
					(READ_BIT(pll_config, RCC_PLLCFGR_PLLQ) != RCC_OscInitStruct->PLL.PLLQ))
				{
					return HAL_ERROR;
				}
			}
		}
	}

	return HAL_OK;
}

static void init_bss(void)
{
	uint32_t *p = &__bss_start;
	uint32_t *e = &__bss_end;
	while(p < e)
	{
		*p++ = 0;
	}
}

void its_board_init(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
	RCC->CR |= 0x00000001;
	RCC->CFGR = 0x00000000;
	RCC->CR &= 0xFEF6FFFF;
	RCC->PLLCFGR = 0x24003010;
	RCC->CR &= 0xFFFBFFFF;
	RCC->CIR = 0x00000000;
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;

	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	__NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
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
	if(_HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		panic();
	}

	_HAL_PWREx_EnableOverDrive();

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
		RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(_HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		panic();
	}

	/* Ports D, E, F, G */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/* Ports D, E Output */

	/* BL: D15, CS: D14 */
	GPIOD->MODER = 0x50005555;
	GPIOD->BSRR = (1 << 15);

	/* DC: F13, RST: F12 */
	GPIOF->MODER = 0x05000000;
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

	spi_ll_init();

	init_bss();
}
