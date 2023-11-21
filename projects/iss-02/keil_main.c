#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "iks01a3.h"
#include "timer.h"

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

	printf("> Demo\n\n");

	for(;;)
	{
		printf("\nvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
		acc_gyro.get_a_axes(axes);
		printf("LSM6DSO [acc/mg]:      %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
		acc_gyro.get_g_axes(axes);
		printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
		accelerometer.get_a_axes(axes);
		printf("LIS2DW12 [acc/mg]:     %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
		magnetometer.get_m_axes(axes);
		printf("LIS2MDL [mag/mgauss]:  %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);

		printf("---\r\n");

		hum_temp.get_temp(&val1);
		hum_temp.get_hum(&val2);
		printf("HTS221:  [temp] %7s C,   [hum] %s%%\n",
			print_double(buf1, val1, 2),
			print_double(buf2, val2, DECIMAL_DIGITS));

		press_temp.get_temp(&val1);
		press_temp.get_press(&val2);
		printf("LPS22HH: [temp] %7s C, [press] %s mbar\n",
			print_double(buf1, val1, 2),
			print_double(buf2, val2, DECIMAL_DIGITS));

		temp.get_temp(&val1);
		printf("STTS751: [temp] %7s C\r\n",
			print_double(buf1, val1, DECIMAL_DIGITS));
		printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		delay(1500);
	}
}

void l_csv(void)
{
	int32_t axes[12];
	printf("> CSV\n\n");
	for(;;)
	{
		acc_gyro.get_a_axes(&axes[0]);
		acc_gyro.get_g_axes(&axes[3]);
		accelerometer.get_a_axes(&axes[6]);
		magnetometer.get_m_axes(&axes[9]);
		printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
			axes[0], axes[1], axes[2], axes[3], axes[4], axes[5],
			axes[6], axes[7], axes[8], axes[9], axes[10], axes[11]);
		delay(1500);
	}
}

void l_joystick(void)
{
	printf("> Joystick\n\n");
	for(;;)
	{
	}
}

void l_alarm(void)
{
	printf("> Alarmanlage\n\n");
	for(;;)
	{
	}
}

#define NUM_SAMPLES  1024

void l_buffer(void)
{
	static int32_t samples[NUM_SAMPLES * 13];
	int32_t *cur;
	int32_t i;

	printf("> Buffer\n\n");
	for(;;)
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

		printf("-----------------------\n");
		cur = samples;
		for(i = 0; i < NUM_SAMPLES; ++i)
		{
			printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
				cur[0], cur[1], cur[2], cur[3], cur[4], cur[5],
				cur[6], cur[7], cur[8], cur[9], cur[10], cur[11], cur[12]);
			cur += 13;
		}
		printf("-----------------------\n");
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

	printf("\n--- Starting new run ---\n");
	acc_gyro.read_id(&id);
	printf("LSM6DSO accelerometer & gyroscope = 0x%X\n", id);
	accelerometer.read_id(&id);
	printf("LIS2DW12 accelerometer            = 0x%X\n", id);
	magnetometer.read_id(&id);
	printf("LIS2MDL magnetometer              = 0x%X\n", id);
	hum_temp.read_id(&id);
	printf("HTS221  humidity & temperature    = 0x%X\n", id);
	press_temp.read_id(&id);
	printf("LPS22HH  pressure & temperature   = 0x%X\n", id);
	temp.read_id(&id);
	printf("STTS751 temperature               = 0x%X\n", id);

	printf("\nPress button to choose option:\n"
		"  S7: CSV Ausgabe\n"
		"  S6: Joystick\n"
		"  S5: Alarmanlage\n"
		"  S4: Buffer\n"
		"  S0: Demo program\n\n");

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
		else if(!(btns & 0x01))
		{
			l_demo();
		}
	}
}
