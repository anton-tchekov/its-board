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
#define TICKS_PER_US   90
#define TICKS_PER_MS     (TICKS_PER_US * 1000)
#define TICKS_PER_S      (TICKS_PER_MS * 1000)

/**
 * @brief This function initializes TIM2
 */
void timer_init(void);

/**
 * @brief This function gets the current timer value
 * @return TIM2 timer ticks
 */
static inline uint32_t timer_get(void)
{
	return TIM2->CNT;
}

void timer_delta_str(char *str, uint32_t dt);

#endif /* __TIMER_H__ */
