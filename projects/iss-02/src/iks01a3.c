/**
 ******************************************************************************
 * @file    iks01a3.c
 * @author  Tobias De Gasperis
 *          HAW-Hamburg
 *          Labor f�r technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    29. April 2022
 * @brief   Funktionen f�r das IKS01A3-Sensorboard basierend auf dem Code
 *          der X-CUBE_MEMS MEMS-App.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "iks01a3.h"
#include <stdio.h>

IKS01A3_Acc_Gyro_t acc_gyro =
{
	NULL,
	acc_gyro_init,
	acc_gyro_enable_acc,
	acc_gyro_enable_gyro,
	acc_gyro_read_id,
	acc_gyro_get_a_axes,
	acc_gyro_get_g_axes,
};

IKS01A3_Accelerometer_t accelerometer =
{
	NULL,
	accel_init,
	accel_enable_acc,
	accel_read_id,
	accel_get_a_axes,
};

IKS01A3_Magnetometer_t magnetometer =
{
	NULL,
	mag_init,
	mag_enable_acc,
	mag_read_id,
	mag_get_a_axes,
};

IKS01A3_Hum_Temp_t hum_temp =
{
	NULL,
	hum_temp_init,
	hum_temp_enable_hum,
	hum_temp_enable_temp,
	hum_temp_read_id,
	hum_temp_get_hum,
	hum_temp_get_temp,
};

IKS01A3_Press_Temp_t press_temp =
{
	NULL,
	press_temp_init,
	press_temp_enable_press,
	press_temp_enable_temp,
	press_temp_read_id,
	press_temp_get_press,
	press_temp_get_temp,
};

IKS01A3_Temp_t temp =
{
	NULL,
	temp_init,
	temp_enable_temp,
	temp_read_id,
	temp_get_temp,
};

int32_t acc_gyro_init(LSM6DSO_Object_t* lsm6dso_obj)
{
	LSM6DSO_IO_t            io_ctx;
	uint8_t                 id;
	LSM6DSO_Capabilities_t  cap;
	int32_t ret = BSP_ERROR_NONE;

	if(NULL == lsm6dso_obj)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	{
		acc_gyro.pLsm6dso_obj = lsm6dso_obj;
		/* Configure the accelero driver */
		io_ctx.BusType     = LSM6DSO_I2C_BUS; /* I2C */
		io_ctx.Address     = LSM6DSO_I2C_ADD_H;
		io_ctx.Init        = IKS01A3_I2C_Init;
		io_ctx.DeInit      = IKS01A3_I2C_DeInit;
		io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
		io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
		io_ctx.GetTick     = IKS01A3_GetTick;

		if (LSM6DSO_RegisterBusIO(lsm6dso_obj, &io_ctx) != LSM6DSO_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (LSM6DSO_ReadID(lsm6dso_obj, &id) != LSM6DSO_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (id != LSM6DSO_ID)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else
		{
			(void)LSM6DSO_GetCapabilities(lsm6dso_obj, &cap);
			if ((ret == BSP_ERROR_NONE) && (cap.Gyro == 1U))
			{
				if(LSM6DSO_COMMON_Driver.Init(lsm6dso_obj) != LSM6DSO_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
						ret = BSP_ERROR_NONE;
				}
			}
			if ((ret == BSP_ERROR_NONE) && (cap.Acc == 1U))
			{
				if (LSM6DSO_COMMON_Driver.Init(lsm6dso_obj) != LSM6DSO_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
		}
	}
  return ret;
}

int32_t acc_gyro_enable_acc(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (LSM6DSO_ACC_Enable(acc_gyro.pLsm6dso_obj) != LSM6DSO_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t acc_gyro_enable_gyro(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (LSM6DSO_GYRO_Enable(acc_gyro.pLsm6dso_obj) != BSP_ERROR_NONE)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t acc_gyro_read_id(uint8_t* id)
{
	return LSM6DSO_ReadID(acc_gyro.pLsm6dso_obj, id);
}

int32_t acc_gyro_get_a_axes(int32_t* acceleration)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	LSM6DSO_Axes_t Acceleration;
	if(NULL == acceleration)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(LSM6DSO_ERROR != LSM6DSO_ACC_GetAxes(acc_gyro.pLsm6dso_obj, &Acceleration))
	{
		ret = BSP_ERROR_NONE;
		acceleration[0] = Acceleration.x;
		acceleration[1] = Acceleration.y;
		acceleration[2] = Acceleration.z;
	}
	return ret;

}

int32_t acc_gyro_get_g_axes(int32_t* angularRate)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	LSM6DSO_Axes_t AngularRate;
	if(NULL == angularRate)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	if(LSM6DSO_ERROR != LSM6DSO_GYRO_GetAxes(acc_gyro.pLsm6dso_obj, &AngularRate)){
		ret = BSP_ERROR_NONE;
		angularRate[0] = AngularRate.x;
		angularRate[1] = AngularRate.y;
		angularRate[2] = AngularRate.z;
	}
	return ret;
}


int32_t accel_init(LIS2DW12_Object_t* pObj)
{
	LIS2DW12_IO_t            io_ctx;
  uint8_t                  id;
  LIS2DW12_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;
	if(NULL == pObj)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	{
		accelerometer.pLis2DW12_obj = pObj;

		/* Configure the accelero driver */
		io_ctx.BusType     = LIS2DW12_I2C_BUS; /* I2C */
		io_ctx.Address     = LIS2DW12_I2C_ADD_H;
		io_ctx.Init        = IKS01A3_I2C_Init;
		io_ctx.DeInit      = IKS01A3_I2C_DeInit;
		io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
		io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
		io_ctx.GetTick     = IKS01A3_GetTick;

		if (LIS2DW12_RegisterBusIO(pObj, &io_ctx) != LIS2DW12_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (LIS2DW12_ReadID(pObj, &id) != LIS2DW12_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (id != LIS2DW12_ID)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else
		{
			(void)LIS2DW12_GetCapabilities(pObj, &cap);
			if ((ret == BSP_ERROR_NONE) && (cap.Acc == 1U))
			{
				if (LIS2DW12_Init(pObj) != LIS2DW12_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
		}
	}
  return ret;
}

int32_t accel_enable_acc(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (LIS2DW12_ACC_Enable(accelerometer.pLis2DW12_obj) != LIS2DW12_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t accel_read_id(uint8_t *id)
{
	return LIS2DW12_ReadID(accelerometer.pLis2DW12_obj, id);
}

int32_t accel_get_a_axes(int32_t *acceleration)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	LIS2DW12_Axes_t Acceleration;
	if(NULL == acceleration)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(LIS2DW12_ERROR != LIS2DW12_ACC_GetAxes(accelerometer.pLis2DW12_obj, &Acceleration))
	{
		ret = BSP_ERROR_NONE;
		acceleration[0] = Acceleration.x;
		acceleration[1] = Acceleration.y;
		acceleration[2] = Acceleration.z;
	}
	return ret;
}

int32_t mag_init(LIS2MDL_Object_t* pObj)
{
	LIS2MDL_IO_t            io_ctx;
  uint8_t                 id;
  LIS2MDL_Capabilities_t  cap;
  int32_t                 ret = BSP_ERROR_NONE;
	if(NULL == pObj)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	{
		magnetometer.pLis2MDL_obj = pObj;

		/* Configure the accelero driver */
		io_ctx.BusType     = LIS2MDL_I2C_BUS; /* I2C */
		io_ctx.Address     = LIS2MDL_I2C_ADD;
		io_ctx.Init        = IKS01A3_I2C_Init;
		io_ctx.DeInit      = IKS01A3_I2C_DeInit;
		io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
		io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
		io_ctx.GetTick     = IKS01A3_GetTick;

		if (LIS2MDL_RegisterBusIO(pObj, &io_ctx) != LIS2MDL_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (LIS2MDL_ReadID(pObj, &id) != LIS2MDL_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (id != LIS2MDL_ID)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else
		{
			(void)LIS2MDL_GetCapabilities(pObj, &cap);
			if ((ret == BSP_ERROR_NONE) && (cap.Magneto == 1U))
			{
				if (LIS2MDL_Init(pObj) != LIS2MDL_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
			else
			{
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
		}
	}
  return ret;
}

int32_t mag_enable_acc(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (LIS2MDL_MAG_Enable(magnetometer.pLis2MDL_obj) != LIS2MDL_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t mag_read_id(uint8_t *id)
{
	return LIS2MDL_ReadID(magnetometer.pLis2MDL_obj, id);
}

int32_t mag_get_a_axes(int32_t *magneticField)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	LIS2MDL_Axes_t MagneticField;
	if(NULL == magneticField)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(LIS2MDL_ERROR != LIS2MDL_MAG_GetAxes(magnetometer.pLis2MDL_obj, &MagneticField))
	{
		ret = BSP_ERROR_NONE;
		magneticField[0] = MagneticField.x;
		magneticField[1] = MagneticField.y;
		magneticField[2] = MagneticField.z;
	}
	return ret;
}










int32_t hum_temp_init(HTS221_Object_t* pObj)
{
	HTS221_IO_t            io_ctx;
  uint8_t              id;
  int32_t              ret = BSP_ERROR_NONE;
  HTS221_Capabilities_t  cap;
	if(NULL == pObj)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	{
		hum_temp.pObj = pObj;
		/* Configure the environmental sensor driver */
		io_ctx.BusType     = HTS221_I2C_BUS; /* I2C */
		io_ctx.Address     = HTS221_I2C_ADDRESS;
		io_ctx.Init        = IKS01A3_I2C_Init;
		io_ctx.DeInit      = IKS01A3_I2C_DeInit;
		io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
		io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
		io_ctx.GetTick     = IKS01A3_GetTick;

		if (HTS221_RegisterBusIO(pObj, &io_ctx) != HTS221_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (HTS221_ReadID(pObj, &id) != HTS221_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (id != HTS221_ID)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else
		{
			(void)HTS221_GetCapabilities(pObj, &cap);

			if ((ret == BSP_ERROR_NONE) && (cap.Temperature == 1U))
			{
				if (HTS221_Init(pObj) != HTS221_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
			if ((ret == BSP_ERROR_NONE) && (cap.Humidity == 1U))
			{
				if (HTS221_Init(pObj) != HTS221_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
			if ((ret == BSP_ERROR_NONE) && !pObj->is_initialized)
			{
				/* Return an error if the application has not initialized the component */
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
		}
	}
  return ret;
}

int32_t hum_temp_enable_hum(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (HTS221_HUM_Enable(hum_temp.pObj) != HTS221_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t hum_temp_enable_temp(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (HTS221_TEMP_Enable(hum_temp.pObj) != HTS221_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t hum_temp_read_id(uint8_t *id)
{
	return HTS221_ReadID(hum_temp.pObj, id);
}

int32_t hum_temp_get_hum(float* humidity)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	if(NULL == humidity)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(HTS221_ERROR != HTS221_HUM_GetHumidity(hum_temp.pObj, humidity))
	{
		ret = BSP_ERROR_NONE;
	}
	return ret;
}

int32_t hum_temp_get_temp(float* temperature)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	if(NULL == temperature)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(HTS221_ERROR != HTS221_TEMP_GetTemperature(hum_temp.pObj, temperature))
	{
		ret = BSP_ERROR_NONE;
	}
	return ret;
}

int32_t press_temp_init(LPS22HH_Object_t* pObj)
{
	LPS22HH_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  LPS22HH_Capabilities_t  cap;
	if(NULL == pObj)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	{
		press_temp.pObj = pObj;
		/* Configure the pressure driver */
		io_ctx.BusType     = LPS22HH_I2C_BUS; /* I2C */
		io_ctx.Address     = LPS22HH_I2C_ADD_H;
		io_ctx.Init        = IKS01A3_I2C_Init;
		io_ctx.DeInit      = IKS01A3_I2C_DeInit;
		io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
		io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
		io_ctx.GetTick     = IKS01A3_GetTick;

		if (LPS22HH_RegisterBusIO(pObj, &io_ctx) != LPS22HH_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (LPS22HH_ReadID(pObj, &id) != LPS22HH_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (id != LPS22HH_ID)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else
		{
			(void)LPS22HH_GetCapabilities(pObj, &cap);

			if ((ret == BSP_ERROR_NONE) && (cap.Temperature == 1U))
			{
				if (LPS22HH_Init(pObj) != LPS22HH_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
			if ((ret == BSP_ERROR_NONE) && (cap.Pressure == 1U))
			{
				if (LPS22HH_Init(pObj) != LPS22HH_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
			if ((ret == BSP_ERROR_NONE) && !pObj->is_initialized)
			{
				/* Return an error if the application has not initialized the component */
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
		}
	}
  return ret;
}

int32_t press_temp_enable_press(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (LPS22HH_PRESS_Enable(press_temp.pObj) != LPS22HH_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t press_temp_enable_temp(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (LPS22HH_TEMP_Enable(press_temp.pObj) != LPS22HH_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t press_temp_read_id(uint8_t *id)
{
	return LPS22HH_ReadID(press_temp.pObj, id);
}

int32_t press_temp_get_press(float* pressure)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	if(NULL == pressure)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(LPS22HH_ERROR != LPS22HH_PRESS_GetPressure(press_temp.pObj, pressure))
	{
		ret = BSP_ERROR_NONE;
	}
	return ret;
}

int32_t press_temp_get_temp(float* temperature)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	if(NULL == temperature)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(LPS22HH_ERROR != LPS22HH_TEMP_GetTemperature(press_temp.pObj, temperature))
	{
		ret = BSP_ERROR_NONE;
	}
	return ret;
}

int32_t temp_init(STTS751_Object_t* pObj)
{
	STTS751_IO_t            io_ctx;
  uint8_t                 id;
  int32_t                 ret = BSP_ERROR_NONE;
  STTS751_Capabilities_t  cap;

	if(NULL == pObj)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else
	{
		temp.pObj = pObj;
		/* Configure the pressure driver */
		io_ctx.BusType     = STTS751_I2C_BUS; /* I2C */
		io_ctx.Address     = STTS751_1xxxx_ADD_7K5;
		io_ctx.Init        = IKS01A3_I2C_Init;
		io_ctx.DeInit      = IKS01A3_I2C_DeInit;
		io_ctx.ReadReg     = IKS01A3_I2C_ReadReg;
		io_ctx.WriteReg    = IKS01A3_I2C_WriteReg;
		io_ctx.GetTick     = IKS01A3_GetTick;

		if (STTS751_RegisterBusIO(pObj, &io_ctx) != STTS751_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (STTS751_ReadID(pObj, &id) != STTS751_OK)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else if (id != STTS751_ID_MAN)
		{
			ret = BSP_ERROR_UNKNOWN_COMPONENT;
		}
		else
		{
			(void)STTS751_GetCapabilities(pObj, &cap);
			if ((ret == BSP_ERROR_NONE) && (cap.Temperature == 1U))
			{
				if (STTS751_Init(pObj) != STTS751_OK)
				{
					ret = BSP_ERROR_COMPONENT_FAILURE;
				}
				else
				{
					ret = BSP_ERROR_NONE;
				}
			}
			else
			{
				/* Return an error if the application has not initialized the component */
				ret = BSP_ERROR_COMPONENT_FAILURE;
			}
		}
	}
  return ret;
}

int32_t temp_enable_temp(void)
{
	int32_t ret = BSP_ERROR_NONE;
	if (STTS751_TEMP_Enable(temp.pObj) != STTS751_OK)
	{
		ret = BSP_ERROR_COMPONENT_FAILURE;
	}
	return ret;
}

int32_t temp_read_id(uint8_t *id)
{
	return STTS751_ReadID(temp.pObj, id);
}

int32_t temp_get_temp(float* temperature)
{
	int32_t ret = BSP_ERROR_COMPONENT_FAILURE;
	if(NULL == temperature)
	{
		ret = BSP_ERROR_WRONG_PARAM;
	}
	else if(STTS751_ERROR != STTS751_TEMP_GetTemperature(temp.pObj, temperature))
	{
		ret = BSP_ERROR_NONE;
	}
	return ret;
}
