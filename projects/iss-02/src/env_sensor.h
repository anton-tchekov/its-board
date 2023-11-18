#ifndef ENV_SENSOR_H
#define ENV_SENSOR_H

#include <stdint.h>

typedef struct
{
  int32_t (*Init)(void *);
  int32_t (*DeInit)(void *);
  int32_t (*ReadID)(void *, uint8_t *);
  int32_t (*GetCapabilities)(void *, void *);
} ENV_SENSOR_CommonDrv_t;

typedef struct
{
  int32_t (*Enable)(void *);
  int32_t (*Disable)(void *);
  int32_t (*GetOutputDataRate)(void *, float *);
  int32_t (*SetOutputDataRate)(void *, float);
  int32_t (*GetValue)(void *, float *);
} ENV_SENSOR_FuncDrv_t;

#endif /* ENV_SENSOR_H */
