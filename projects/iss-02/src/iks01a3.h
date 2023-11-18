/**
 ******************************************************************************
 * @file    iks01a3.h
 * @author  Tobias De Gasperis
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    29. April 2022
 * @brief   Funktionen für das IKS01A3-Sensorboard basierend auf dem Code 
 *          der X-CUBE_MEMS MEMS-App.
 ******************************************************************************
 */

#ifndef _IKS01A3_H
#define	_IKS01A3_H

#include "iks01a3_motion_sensors.h"
#include "iks01a3_env_sensors.h"

/* Motion Sensor instance Info */
typedef struct
{
  LSM6DSO_Object_t acc_gyro;
	LIS2DW12_Object_t acc;
	LIS2MDL_Object_t mag;
  HTS221_Object_t hum_temp;
	LPS22HH_Object_t press_temp;
	STTS751_Object_t temp;
} IKS01A3_SENSORS_t;

typedef struct
{
	LSM6DSO_Object_t* pLsm6dso_obj;
	int32_t (*init)(LSM6DSO_Object_t* lsm6dso_obj);
	int32_t (*enable_acc)(void);
	int32_t (*enable_gyr)(void);
	int32_t (*read_id)(uint8_t* id);
	int32_t (*get_a_axes)(int32_t* axes);
	int32_t (*get_g_axes)(int32_t* axes);
} IKS01A3_Acc_Gyro_t;

typedef struct
{
	LIS2DW12_Object_t* pLis2DW12_obj;
	int32_t (*init)(LIS2DW12_Object_t* pObj);
	int32_t (*enable_acc)(void);
	int32_t (*read_id)(uint8_t* id);
	int32_t (*get_a_axes)(int32_t* axes);
} IKS01A3_Accelerometer_t;


typedef struct
{
	LIS2MDL_Object_t* pLis2MDL_obj;
	int32_t (*init)(LIS2MDL_Object_t* pObj);
	int32_t (*enable_mag)(void);
	int32_t (*read_id)(uint8_t* id);
	int32_t (*get_m_axes)(int32_t* axes);
} IKS01A3_Magnetometer_t;

typedef struct
{
	HTS221_Object_t* pObj;
	int32_t (*init)(HTS221_Object_t* pObj);
	int32_t (*enable_hum)(void);
	int32_t (*enable_temp)(void);
	int32_t (*read_id)(uint8_t* id);
	int32_t (*get_hum)(float* humidity);
	int32_t (*get_temp)(float* temperature);
} IKS01A3_Hum_Temp_t;

typedef struct
{
	LPS22HH_Object_t* pObj;
	int32_t (*init)(LPS22HH_Object_t* pObj);
	int32_t (*enable_press)(void);
	int32_t (*enable_temp)(void);
	int32_t (*read_id)(uint8_t* id);
	int32_t (*get_press)(float* pressure);
	int32_t (*get_temp)(float* temperature);
} IKS01A3_Press_Temp_t;

typedef struct
{
	STTS751_Object_t* pObj;
	int32_t (*init)(STTS751_Object_t* pObj);
	int32_t (*enable_temp)(void);
	int32_t (*read_id)(uint8_t* id);
	int32_t (*get_temp)(float* temperature);
} IKS01A3_Temp_t;

int32_t acc_gyro_init(LSM6DSO_Object_t* lsm6dso_obj);
int32_t acc_gyro_enable_acc(void);
int32_t acc_gyro_enable_gyro(void);
int32_t acc_gyro_read_id(uint8_t *Id);
int32_t acc_gyro_get_a_axes(int32_t *acceleration);
int32_t acc_gyro_get_g_axes(int32_t *angularRate);

int32_t accel_init(LIS2DW12_Object_t* pObj);
int32_t accel_enable_acc(void);
int32_t accel_read_id(uint8_t *id);
int32_t accel_get_a_axes(int32_t *acceleration);

int32_t mag_init(LIS2MDL_Object_t* pObj);
int32_t mag_enable_acc(void);
int32_t mag_read_id(uint8_t *id);
int32_t mag_get_a_axes(int32_t *magneticField);

int32_t hum_temp_init(HTS221_Object_t* pObj);
int32_t hum_temp_enable_hum(void);
int32_t hum_temp_enable_temp(void);
int32_t hum_temp_read_id(uint8_t *id);
int32_t hum_temp_get_hum(float* humidity);
int32_t hum_temp_get_temp(float* temperature);

int32_t press_temp_init(LPS22HH_Object_t* pObj);
int32_t press_temp_enable_press(void);
int32_t press_temp_enable_temp(void);
int32_t press_temp_read_id(uint8_t *id);
int32_t press_temp_get_press(float* pressure);
int32_t press_temp_get_temp(float* temperature);

int32_t temp_init(STTS751_Object_t* pObj);
int32_t temp_enable_temp(void);
int32_t temp_read_id(uint8_t *id);
int32_t temp_get_temp(float* temperature);


extern IKS01A3_Acc_Gyro_t acc_gyro;
extern IKS01A3_Accelerometer_t accelerometer;
extern IKS01A3_Magnetometer_t magnetometer;
extern IKS01A3_Hum_Temp_t hum_temp;
extern IKS01A3_Press_Temp_t press_temp;
extern IKS01A3_Temp_t temp;

#endif

//EOF
