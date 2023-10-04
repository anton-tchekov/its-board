#include <stdlib.h>

static int _seed;

int rand(void)
{
	_seed ^= (_seed << 13);
	_seed ^= (_seed >> 17);
	_seed ^= (_seed << 5);
	return _seed;
}

void srand(int seed)
{
	_seed = seed;
}
