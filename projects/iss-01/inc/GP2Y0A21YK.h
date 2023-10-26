#ifndef GP2Y0A21YK_H
#define GP2Y0A21YK_H

#include <stdint.h>

#define ANALOG_PIN_MUX (TODO)

/** TODO
 * @brief reads the voltage of the sensor via the defined adc pin
 * @retval voltage * 10mv
*/
int32_t GP2_read_voltage();

/**
 * @brief calculates the distance the sensor measures, by the given voltage
 * @param voltage the voltage which you received by the sensor
 * @retval the distance in mm
*/
int32_t GP2_get_distance(int32_t voltage);


#endif