#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <stdint.h>

typedef struct
{
  int32_t (*Init)(void *);
  int32_t (*DeInit)(void *);
  int32_t (*ReadID)(void *, uint8_t *);
  int32_t (*GetCapabilities)(void *, void *);
} MOTION_SENSOR_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(void *);
  int32_t (*Disable)(void *);
  int32_t (*GetSensitivity)(void *, float *);
  int32_t (*GetOutputDataRate)(void *, float *);
  int32_t (*SetOutputDataRate)(void *, float);
  int32_t (*GetFullScale)(void *, int32_t *);
  int32_t (*SetFullScale)(void *, int32_t);
  int32_t (*GetAxes)(void *, void *);
  int32_t (*GetAxesRaw)(void *, void *);
} MOTION_SENSOR_FuncDrv_t;

#endif
