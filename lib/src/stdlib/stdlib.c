#include <stdlib.h>

static int _seed;

int rand(void)
{
	return 0;
}

void srand(int seed)
{
	_seed = seed;
}
