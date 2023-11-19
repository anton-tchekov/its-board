#include <stdint.h>
#include "timer.h"

uint32_t SystemCoreClock = 180000000;

uint32_t HAL_RCC_GetPCLK1Freq(void)
{
	return 45000000;
}

uint32_t HAL_GetTick(void)
{
	return timer_get() / TICKS_PER_MS;
}
