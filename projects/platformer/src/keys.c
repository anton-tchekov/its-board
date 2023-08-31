#include "keys.h"
#include "stm32f4xx_hal.h"

#define PIN_KEY_UP     (1 << 6)
#define PIN_KEY_LEFT   (1 << 7)
#define PIN_KEY_RIGHT  (1 << 5)

int keys_get(void)
{
	int port = GPIOF->IDR;
	int keys = 0;
	if(!(port & PIN_KEY_UP))
	{
		keys |= KEY_MASK_UP;
	}

	if(!(port & PIN_KEY_LEFT))
	{
		keys |= KEY_MASK_LEFT;
	}

	if(!(port & PIN_KEY_RIGHT))
	{
		keys |= KEY_MASK_RIGHT;
	}

	return keys;
}
