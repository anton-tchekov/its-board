/**
 * @file    adc.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 * @brief   ADC functions
 */

#ifndef __ADC_H__
#define __ADC_H__

/**
 * @brief Initialize an ADC
 *
 * @param adc ADC number (0 - 2)
 */
void adc_init(uint32_t adc);

/**
 * @brief Read an ADC channel
 *
 * @param adc ADC number (0 - 2)
 * @param channel ADC channel (0 - 15)
 * @return ADC value 0 - 1023, -1 on timeout
 */
int32_t adc_read(uint32_t adc, uint32_t channel);

/**
 * @brief Convert ADC value to voltage in millivolts
 *
 * @param adc_value ADC value (0 - 4095)
 * @return Voltage in millivolts (0 - 3300)
 */
static inline int32_t adc_to_mv(int32_t adc_value)
{
	return (adc_value * 3300) / 4095;
}

#endif /* __ADC_H__ */
