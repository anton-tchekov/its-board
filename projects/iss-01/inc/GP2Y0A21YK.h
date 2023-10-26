/**
 * @file    GP2Y0A21YK.h
 * @author  Haron Nazari
 * @version 0.1
 * @date    2023-10-26
 * @brief   GP2Y0A21YK distance sensor
 */

#ifndef __GP2Y0A21YK_H__
#define __GP2Y0A21YK_H__

#include <stdint.h>

/**
 * @brief Calculates the distance the sensor measures, by the given voltage
 *
 * @param voltage The voltage which you received by the sensor in 10 mV units
 * @retval The distance in mm
 */
int32_t gp2_get_distance(uint32_t voltage);

#endif /* __GP2Y0A21YK_H__ */
