#include "m_swap.h"

#ifndef __BIG_ENDIAN__

unsigned short SwapSHORT(unsigned short x)
{
	return (x >> 8) | (x << 8);
}

unsigned long SwapLONG( unsigned long x)
{
    return
	(x>>24)
	| ((x>>8) & 0xff00)
	| ((x<<8) & 0xff0000)
	| (x<<24);
}

#endif
