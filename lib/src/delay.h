/**
 * @file    delay.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 * @brief   Delay functions
 */

#ifndef __DELAY_H__
#define __DELAY_H__

#include <stdint.h>

/**
 * @brief Delay for the specified amount of microseconds
 * @param us Microseconds
 */
void delay_us(uint32_t us);

/**
 * @brief Delay for the specified amount of milliseconds
 * @param ms Milliseconds
 */
void delay_ms(uint32_t ms);

#endif /* __DELAY_H__ */
