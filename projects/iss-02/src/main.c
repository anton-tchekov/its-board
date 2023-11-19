/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 * @brief   Template Project
 */

#include "init.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#include "uart.h"
#include "iks01a3.h"
#include <stdio.h>

#define NUMBER_OF_AXES	3
#define SIZE_OF_BUFFER   128
#define DECIMAL_DIGITS	2

char *print_double(char *str, double v, int decimalDigits)
{
	int i = 1;
	int intPart, fractPart;
	int len;
	char *ptr;

	/* prepare decimal digits multiplicator */
	for(; decimalDigits != 0; i *= 10, decimalDigits--) ;

	/* calculate integer & fractinal parts */
	intPart = (int)v;
	fractPart = (int)((v - (double)(int)v) * i);

	/* fill in integer part */
	sprintf(str, "%i.", intPart);

	/* prepare fill in of fractional part */
	len = strlen(str);
	ptr = &str[len];

	/* fill in leading fractional zeros */
	for(i /= 10; i > 1; i /= 10, ptr++)
	{
		if(fractPart >= i)
		{
			break;
		}

		*ptr = '0';
	}

	/* fill in (rest of) fractional part */
	sprintf(ptr, "%i", fractPart);
	return str;
}

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	IKS01A3_SENSORS_t iks;
	uint8_t id = 0;
	int32_t axes[NUMBER_OF_AXES] = {0};
	float value1 = 0.0f;
	float value2 = 0.0f;
	char buf[SIZE_OF_BUFFER];
	char buf1[SIZE_OF_BUFFER];
	char buf2[SIZE_OF_BUFFER];

	its_board_init();
	timer_init();
	uart_init(115200);
	uart_tx_str("ISS-02 Init\n");

	acc_gyro.init(&iks.acc_gyro);
	uart_tx_str("acc_gyro Init\n");

	accelerometer.init(&iks.acc);
	uart_tx_str("accelerometer Init\n");

	magnetometer.init(&iks.mag);
	uart_tx_str("magnetometer Init\n");

	hum_temp.init(&iks.hum_temp);
	uart_tx_str("hum_temp Init\n");

	press_temp.init(&iks.press_temp);
	uart_tx_str("press_temp Init\n");

	temp.init(&iks.temp);

	uart_tx_str("Init done\n");

	acc_gyro.enable_acc();
	acc_gyro.enable_gyr();
	accelerometer.enable_acc();
	magnetometer.enable_mag();
	hum_temp.enable_hum();
	hum_temp.enable_temp();
	press_temp.enable_press();
	press_temp.enable_temp();
	temp.enable_temp();

	uart_tx_str("Enable done\n");

	acc_gyro.read_id(&id);
	sprintf(buf, "LSM6DSO accelerometer & gyroscope = 0x%x\n", id);
	uart_tx_str(buf);

	accelerometer.read_id(&id);
	sprintf(buf, "LIS2DW12 accelerometer            = 0x%x\n", id);
	uart_tx_str(buf);

	magnetometer.read_id(&id);
	sprintf(buf, "LIS2MDL magnetometer              = 0x%x\n", id);
	uart_tx_str(buf);

	for(;;)
	{
		uart_tx_str("\nvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");

		acc_gyro.get_a_axes(axes);
		sprintf(buf, "LSM6DSO [acc/mg]:      %6d, %6d, %6d\n",
			axes[0], axes[1], axes[2]);
		uart_tx_str(buf);

		acc_gyro.get_g_axes(axes);
		sprintf(buf, "LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\n",
			axes[0], axes[1], axes[2]);
		uart_tx_str(buf);

		accelerometer.get_a_axes(axes);
		sprintf(buf, "LIS2DW12 [acc/mg]:     %6d, %6d, %6d\n",
			axes[0], axes[1], axes[2]);
		uart_tx_str(buf);

		magnetometer.get_m_axes(axes);
		sprintf(buf, "LIS2MDL [mag/mgauss]:  %6d, %6d, %6d\n",
			axes[0], axes[1], axes[2]);
		uart_tx_str(buf);

		uart_tx_str("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		delay_ms(1500);
	}

	return 0;
}
