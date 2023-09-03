#include "ramexec.h"

void ramexec(uint8_t *code)
{
	void (*fn)(void) = code;
	fn();
}
