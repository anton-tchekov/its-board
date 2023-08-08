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

char *strcpy(char *dst, const char *src)
{
	char *save = dst;
	for(; (*dst = *src); ++src, ++dst);
	return save;
}

int strcmp(const char *str1, const char *str2)
{
	while(*str1 == *str2)
	{
		if(!*str1)
		{
			return 0;
		}

		++str1;
		++str2;
	}

	return *str1 - *str2;
}

int strncmp(const char *p1, const char *p2, size_t count)
{
	size_t i;

	for(i = 0; i < count; ++i)
	{
		if(*p1 != *p2)
		{
			return *p1 - *p2;
		}

		if(!*p1)
		{
			return 0;
		}

		++p1;
		++p2;
	}

	return 0;
}
