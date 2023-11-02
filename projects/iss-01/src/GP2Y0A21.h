/**
 * @file    GP2Y0A21.h
 * @author  Haron Nazari, Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 * @brief   GP2Y0A21 distance sensor
 */

#ifndef __GP2Y0A21_H__
#define __GP2Y0A21_H__

#include <stdint.h>

/**
 * @brief Calculates the distance by the given ADC value
 *
 * @param value The ADC value you received from the sensor (0 - 4095)
 * @retval The distance in millimeters
 */
int32_t gp2_get_distance(uint32_t value);

#endif /* __GP2Y0A21_H__ */
