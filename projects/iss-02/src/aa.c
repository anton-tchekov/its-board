/**
 ******************************************************************************
 * @file    main.c
 * @author  Tobias De Gasperis
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    29. April 2022
 * @brief   Rahmen fuer den Zugriff auf das Nucleo IKS01A3-Sensor-Board
 *
 ******************************************************************************
 */

#ifdef AAAA

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "iks01a3.h"

#define NUMBER_OF_AXES 3
#define SIZE_OF_BUFFER 32
#define SIZE_OF_STRING 64
#define DECIMAL_DIGITS 2

/* Helper function for printing floats & doubles
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
char *print_double(char *str, double v, int decimalDigits)
{
    int i = 1;
    int intPart, fractPart;
    int len;
    char *ptr;

    /* prepare decimal digits multiplicator */
    for (; decimalDigits != 0; i *= 10, decimalDigits--);

    /* calculate integer & fractinal parts */
    intPart = (int)v;
    fractPart = (int)((v - (double)(int)v) * i);

    /* fill in integer part */
    sprintf(str, "%i.", intPart);

    /* prepare fill in of fractional part */
    len = strlen(str);
    ptr = &str[len];

    /* fill in leading fractional zeros */
    for (i /= 10; i > 1; i /= 10, ptr++) {
        if (fractPart >= i) {
            break;
        }
        *ptr = '0';
    }

    /* fill in (rest of) fractional part */
    sprintf(ptr, "%i", fractPart);

    return str;
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){

	IKS01A3_SENSORS_t iks;
	uint8_t id = 0;
	int32_t axes[NUMBER_OF_AXES] = {0};
	float value1 = 0.0f;
	float value2 = 0.0f;
	char string[SIZE_OF_STRING] = {0};
  char buffer1[SIZE_OF_BUFFER] = {0};
	char buffer2[SIZE_OF_BUFFER] = {0};

	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}

	lcdPrintlnS("Nucleo IKS01A3");
	lcdPrintlnS("");
	lcdPrintlnS("");

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
	snprintf(string, sizeof(string), "LSM6DSO accelerometer & gyroscope = 0x%X", id);
	printf("%s\r\n", string);
	lcdPrintlnS(string);

	accelerometer.read_id(&id);
	lcdPrintlnS("");
	snprintf(string, sizeof(string), "LIS2DW12 accelerometer            = 0x%X", id);
	printf("%s\r\n", string);
	lcdPrintlnS(string);

	magnetometer.read_id(&id);
	lcdPrintlnS("");
	snprintf(string, sizeof(string), "LIS2MDL magnetometer              = 0x%X", id);
	printf("%s\r\n", string);
	lcdPrintlnS(string);

	hum_temp.read_id(&id);
	lcdPrintlnS("");
	snprintf(string, sizeof(string), "HTS221  humidity & temperature    = 0x%X", id);
	printf("%s\r\n", string);
	lcdPrintlnS(string);

	press_temp.read_id(&id);
	lcdPrintlnS("");
	snprintf(string, sizeof(string), "LPS22HH  pressure & temperature   = 0x%X", id);
	printf("%s\r\n", string);
	lcdPrintlnS(string);

	temp.read_id(&id);
	lcdPrintlnS("");
	snprintf(string, sizeof(string), "STTS751 temperature               = 0x%X", id);
	printf("%s\r\n", string);
	lcdPrintlnS(string);

	while (1) {
		printf("\r\nvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\r\n");

		acc_gyro.get_a_axes(axes);
		printf("LSM6DSO [acc/mg]:      %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
		acc_gyro.get_g_axes(axes);
		printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);

		accelerometer.get_a_axes(axes);
		printf("LIS2DW12 [acc/mg]:     %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);

		magnetometer.get_m_axes(axes);
		printf("LIS2MDL [mag/mgauss]:  %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);

		printf("---\r\n");

		hum_temp.get_temp(&value1);
		hum_temp.get_hum(&value2);
		printf("HTS221:  [temp] %7s C,   [hum] %s%%\r\n", print_double(buffer1, value1, 2), print_double(buffer2, value2, DECIMAL_DIGITS));

		press_temp.get_temp(&value1);
		press_temp.get_press(&value2);
		printf("LPS22HH: [temp] %7s C, [press] %s mbar\r\n", print_double(buffer1, value1, 2), print_double(buffer2, value2, DECIMAL_DIGITS));

		temp.get_temp(&value1);
		printf("STTS751: [temp] %7s C\r\n", print_double(buffer1, value1, DECIMAL_DIGITS));
		printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\r\n");
		delay(1500);
	}
}

// EOF
#endif