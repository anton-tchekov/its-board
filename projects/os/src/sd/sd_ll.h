/**
 * @file    sd_ll.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-25
 * @brief   SD card low-level interface
 */

#ifndef __SD_LL_H__
#define __SD_LL_H__

#include "stm32f4xx_hal.h"

#define SD_LL_CS_BSRR   GPIOE->BSRR
#define SD_LL_CS      11

static inline void sd_ll_select(void)
{
	SD_LL_CS_BSRR |= (1 << (SD_LL_CS + 16));
}

static inline void sd_ll_deselect(void)
{
	SD_LL_CS_BSRR |= (1 << SD_LL_CS);
}

#endif /* __SD_LL_H__ */
