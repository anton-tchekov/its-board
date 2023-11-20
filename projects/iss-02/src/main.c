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

#define SIZE_OF_BUFFER   128

static void check(char *s, int ret)
{
	char buf[SIZE_OF_BUFFER];
	sprintf(buf, "%s : %d\n", s, ret);
	uart_tx_str(buf);
}

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	IKS01A3_SENSORS_t iks;
	uint8_t id = 0;
	int32_t axes[12];
	char buf[SIZE_OF_BUFFER];

	its_board_init();
	timer_init();
	uart_init(115200);
	uart_tx_str("ISS-02 Init\n");

	check("acc_gyro.init", acc_gyro.init(&iks.acc_gyro));
	check("accelerometer.init", accelerometer.init(&iks.acc));
	check("magnetometer.init", magnetometer.init(&iks.mag));
	check("acc_gyro.enable_acc", acc_gyro.enable_acc());
	check("acc_gyro.enable_gyr", acc_gyro.enable_gyr());
	check("accelerometer.enable_acc", accelerometer.enable_acc());
	check("magnetometer.enable_mag", magnetometer.enable_mag());

	check("acc_gyro.read_id", acc_gyro.read_id(&id));
	sprintf(buf, "LSM6DSO accelerometer and gyroscope = 0x%x\n", id);
	uart_tx_str(buf);
	check("accelerometer.read_id", accelerometer.read_id(&id));
	sprintf(buf, "LIS2DW12 accelerometer              = 0x%x\n", id);
	uart_tx_str(buf);
	check("magnetometer.read_id", magnetometer.read_id(&id));
	sprintf(buf, "LIS2MDL magnetometer                = 0x%x\n", id);
	uart_tx_str(buf);

/*
		"LSM6DSO [acc/mg]:"
		"LSM6DSO [gyro/mdps]:"
		"LIS2DW12 [acc/mg]:"
		"LIS2MDL [mag/mgauss]"
*/

	uart_tx_str(" AccX | AccY | AccZ |GyroX |GyroY |GyroZ | AccX | AccY | AccZ | MagX | MagY | MagZ \n");
	for(;;)
	{
		check("acc_gyro.get_a_axes", acc_gyro.get_a_axes(&axes[0]));
		check("acc_gyro.get_g_axes", acc_gyro.get_g_axes(&axes[3]));
		check("accelerometer.get_a_axes", accelerometer.get_a_axes(&axes[6]));
		check("magnetometer.get_m_axes", magnetometer.get_m_axes(&axes[9]));

		sprintf(buf, "%6d|%6d|%6d|%6d|%6d|%6d|%6d|%6d|%6d|%6d|%6d|%6d\n",
			axes[0], axes[1], axes[2], axes[3], axes[4], axes[5],
			axes[6], axes[7], axes[8], axes[9], axes[10], axes[11]);
		uart_tx_str(buf);
		delay_ms(1500);
	}

	return 0;
}
