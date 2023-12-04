#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "iks01a3.h"
#include "timer.h"
#include <limits.h>

#define NUMBER_OF_AXES  3
#define SIZE_OF_BUFFER 32
#define SIZE_OF_STRING 64
#define DECIMAL_DIGITS  2

char *print_double(char *str, double v, int decimalDigits)
{
	int i = 1;
	int intPart, fractPart;
	int len;
	char *ptr;

	for(; decimalDigits != 0; i *= 10, decimalDigits--);
	intPart = (int)v;
	fractPart = (int)((v - (double)(int)v) * i);
	sprintf(str, "%i.", intPart);
	len = strlen(str);
	ptr = &str[len];
	for(i /= 10; i > 1; i /= 10, ptr++)
	{
		if(fractPart >= i)
		{
			break;
		}

		*ptr = '0';
	}

	sprintf(ptr, "%i", fractPart);
	return str;
}

void l_demo(void)
{
	int32_t axes[NUMBER_OF_AXES];
	float val1, val2;
  char buf1[SIZE_OF_BUFFER];
	char buf2[SIZE_OF_BUFFER];
	
	printf("> Demo\r\n\r\n");
	
	for(;;)
	{
		printf("\r\nvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\r\n");
		acc_gyro.get_a_axes(axes);
		printf("LSM6DSO [acc/mg]:      %6d, %6d, %6d\r\r\n", axes[0], axes[1], axes[2]);
		acc_gyro.get_g_axes(axes);
		printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\r\n", axes[0], axes[1], axes[2]);
		accelerometer.get_a_axes(axes);
		printf("LIS2DW12 [acc/mg]:     %6d, %6d, %6d\r\r\n", axes[0], axes[1], axes[2]);
		magnetometer.get_m_axes(axes);
		printf("LIS2MDL [mag/mgauss]:  %6d, %6d, %6d\r\r\n", axes[0], axes[1], axes[2]);

		printf("---\r\r\n");

		hum_temp.get_temp(&val1);
		hum_temp.get_hum(&val2);
		printf("HTS221:  [temp] %7s C,   [hum] %s%%\r\n",
			print_double(buf1, val1, 2),
			print_double(buf2, val2, DECIMAL_DIGITS));

		press_temp.get_temp(&val1);
		press_temp.get_press(&val2);
		printf("LPS22HH: [temp] %7s C, [press] %s mbar\r\n",
			print_double(buf1, val1, 2),
			print_double(buf2, val2, DECIMAL_DIGITS));

		temp.get_temp(&val1);
		printf("STTS751: [temp] %7s C\r\r\n",
			print_double(buf1, val1, DECIMAL_DIGITS));
		printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\r\n");
		delay(1500);
	}
}

void l_csv(void)
{
	int32_t axes[12];
	printf("> CSV\r\n\r\n");
	for(;;)
	{
		acc_gyro.get_a_axes(&axes[0]);
		acc_gyro.get_g_axes(&axes[3]);
		accelerometer.get_a_axes(&axes[6]);
		magnetometer.get_m_axes(&axes[9]);
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
			axes[0], axes[1], axes[2], axes[3], axes[4], axes[5],
			axes[6], axes[7], axes[8], axes[9], axes[10], axes[11]);
	}
}

#define PI 3.14159f
#define RAD_TO_DEG (180.0f / PI)

void l_joystick(void)
{
	printf("> Joystick\r\n\r\n");
	int32_t axes[3];

	float x1, y1, z1;
	float jX, jY;
	
	for(;;)
	{
		acc_gyro.get_a_axes(axes);
		x1 = axes[0] / 1000.0f;
		y1 = axes[1] / 1000.0f;
		z1 = axes[2] / 1000.0f;

		jX = RAD_TO_DEG * (atan2(-y1, -z1) + PI);
		jY = RAD_TO_DEG * (atan2(-x1, -z1) + PI);
		
		if(jX > 270.0f) { jX -= 360.0f; }
		if(jY > 270.0f) { jY -= 360.0f; }
		
		printf("X: %5.2f | Y: %5.2f\r\n", jX, jY);
	}
}

#define BASELINE_SAMPLE 1024
#define BASELINE_SENSI   20

void l_alarm(void)
{
	delay(100);
	int32_t min_acc[3] = { INT_MAX, INT_MAX, INT_MAX };
	int32_t max_acc[3] = { INT_MIN, INT_MIN, INT_MIN };
	int32_t changed_acc[3];
	int32_t i;

	printf("> Alarmanlage\r\n\r\n");

	for(i = 0; i < BASELINE_SAMPLE; ++i)
	{
		acc_gyro.get_a_axes(changed_acc);

		if(changed_acc[0] < min_acc[0]) { min_acc[0] = changed_acc[0]; }
		if(changed_acc[1] < min_acc[1]) { min_acc[1] = changed_acc[1]; }
		if(changed_acc[2] < min_acc[2]) { min_acc[2] = changed_acc[2]; }

		if(changed_acc[0] > max_acc[0]) { max_acc[0] = changed_acc[0]; }
		if(changed_acc[1] > max_acc[1]) { max_acc[1] = changed_acc[1]; }
		if(changed_acc[2] > max_acc[2]) { max_acc[2] = changed_acc[2]; }

		delay(1);
	}

	max_acc[0] += BASELINE_SENSI;
	max_acc[1] += BASELINE_SENSI;
	max_acc[2] += BASELINE_SENSI;

	min_acc[0] -= BASELINE_SENSI;
	min_acc[1] -= BASELINE_SENSI;
	min_acc[2] -= BASELINE_SENSI;

	for(;;)
	{
		acc_gyro.get_a_axes(changed_acc);
		if(changed_acc[0] < min_acc[0] || changed_acc[1] < min_acc[1] || changed_acc[2] < min_acc[2] ||
			changed_acc[0] > max_acc[0] || changed_acc[1] > max_acc[1] || changed_acc[2] > max_acc[2])
		{
			GPIOD->BSRR = (1 << 0);
		}
	}
}

void l_measure(void)
{
	uint8_t id;
	printf("> I2C Oszilloskop Messung\r\n\r\n");
	
	for(;;)
	{
		acc_gyro.read_id(&id);
		delay(1000);
	}
}


#define NUM_SAMPLES  1024

void l_buffer(void)
{
	static int32_t samples[NUM_SAMPLES * 13];
	int32_t *cur;
	int32_t i;

	printf("> Buffer\r\n\r\n");
	//for(;;)
	{
		GPIOD->BSRR |= (1 << 1);
		TIM2->CNT = 0;
		cur = samples;
		for(i = 0; i < NUM_SAMPLES; ++i)
		{
			*cur++ = TIM2->CNT / TICKS_PER_US;
			acc_gyro.get_a_axes(cur);
			cur += 3;
			acc_gyro.get_g_axes(cur);
			cur += 3;
			accelerometer.get_a_axes(cur);
			cur += 3;
			magnetometer.get_m_axes(cur);
			cur += 3;
		}
		GPIOD->BSRR |= (1 << (16 + 1));

		cur = samples;
		printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
		"Sample", "Zeit", "Acc_x", "Acc_y", "Acc_z", "Gyro_x", "Gyro_y", "Gyro_z", "Acc_x2", "Acc_y2", "Acc_z2", "Mag_x", "Mag_y", "Mag_z");
		for(i = 0; i < NUM_SAMPLES; ++i)
		{
			printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
				i, cur[0], cur[1], cur[2], cur[3], cur[4], cur[5],
				cur[6], cur[7], cur[8], cur[9], cur[10], cur[11], cur[12]);
			cur += 13;
		}
	}
}

int main(void)
{
	IKS01A3_SENSORS_t iks;
	uint8_t id = 0;

	initITSboard();
	initTimer();

	acc_gyro.init(&iks.acc_gyro);
	accelerometer.init(&iks.acc);
	magnetometer.init(&iks.mag);
	hum_temp.init(&iks.hum_temp);
	press_temp.init(&iks.press_temp);
	temp.init(&iks.temp);

	acc_gyro.enable_acc();
	acc_gyro.enable_gyr();
	accelerometer.enable_acc();
	magnetometer.enable_mag();
	hum_temp.enable_hum();
	hum_temp.enable_temp();
	press_temp.enable_press();
	press_temp.enable_temp();
	temp.enable_temp();

	printf("\r\n--- Starting new run ---\r\n");
	acc_gyro.read_id(&id);
	printf("LSM6DSO accelerometer & gyroscope = 0x%X\r\n", id);
	accelerometer.read_id(&id);
	printf("LIS2DW12 accelerometer            = 0x%X\r\n", id);
	magnetometer.read_id(&id);
	printf("LIS2MDL magnetometer              = 0x%X\r\n", id);
	hum_temp.read_id(&id);
	printf("HTS221  humidity & temperature    = 0x%X\r\n", id);
	press_temp.read_id(&id);
	printf("LPS22HH  pressure & temperature   = 0x%X\r\n", id);
	temp.read_id(&id);
	printf("STTS751 temperature               = 0x%X\r\n", id);

	printf("\r\nPress button to choose option:\r\n"
		"  S7: CSV Ausgabe\r\n"
		"  S6: Joystick\r\n"
		"  S5: Alarmanlage\r\n"
		"  S4: Buffer\r\n"
		"  S0: Demo program\r\n\r\n");

	for(;;)
	{
		int btns = GPIOF->IDR;
		if(!(btns & 0x80))
		{
			l_csv();
		}
		else if(!(btns & 0x40))
		{
			l_joystick();
		}
		else if(!(btns & 0x20))
		{
			l_alarm();
		}
		else if(!(btns & 0x10))
		{
			l_buffer();
		}
		else if(!(btns & 0x02))
		{
			l_measure();
		}
		else if(!(btns & 0x01))
		{
			l_demo();
		}
	}
}
