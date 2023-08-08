#include <string.h>

size_t strlen(const char *s)
{
	const char *p = s;
	while(*p++) {}
	return p - s - 1;
}

size_t strnlen(const char *s, size_t max)
{
	size_t i;
	for(i = 0; i < max; i++)
	{
		if(s[i] == 0)
		{
			return i;
		}
	}

	return max;
}
