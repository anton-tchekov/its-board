/**
 * @file    timer.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 * @brief   Timer functions
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include "stm32f4xx_hal.h"

/** Ticks per microsecond */
#define TICKS_PER_US 90

/**
 * @brief This function initializes TIM2
 */
void timer_init(void);

/**
 * @brief This function gets the current timer value
 * @return TIM2 timer ticks
 */
inline uint32_t timer_get(void)
{
	return TIM2->CNT;
}

#endif /* __TIMER_H__ */
