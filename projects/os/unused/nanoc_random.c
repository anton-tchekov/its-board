#include "nanoc_random.h"
#include <stdlib.h>

u32r nanoc_srand(u32 *p)
{
	srand(p[0]);
	return 0;
}

u32r nanoc_rand(u32 *p)
{
	return rand();
}
