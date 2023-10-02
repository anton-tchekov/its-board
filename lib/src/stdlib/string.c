#include <string.h>
#include <stdint.h>

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

char *strncpy(char *dest, const char *src, size_t count)
{
	size_t i;
	int c;
	for(i = 0; i < count && (c = src[i]); ++i)
	{
		dest[i] = c;
	}

	return dest;
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

const char *strchr(const char *str, int c)
{
	int d;
	while((d = *str))
	{
		if(d == c)
		{
			return str;
		}

		++str;
	}

	return NULL;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	uint8_t *d, *s, *end;

	s = (uint8_t *)src;
	d = (uint8_t *)dest;
	end = d + count;

	if(d != s)
	{
		while(d != end)
		{
			*d++ = *s++;
		}
	}

	return dest;
}

void *memmove(void *dest, const void *src, size_t count)
{
	uint8_t *d, *s, *end;

	s = (uint8_t *)src;
	d = (uint8_t *)dest;
	end = s + count;

	if(d > s && d < end)
	{
		s = end;
		d += count;
		while(s > (uint8_t *)src)
		{
			*--d = *--s;
		}
	}
	else if(s != d)
	{
		while(s < end)
		{
			*d++ = *s++;
		}
	}

	return dest;
}

void *memset(void *ptr, int value, size_t count)
{
	uint8_t *p, *end;
	for(p = (uint8_t *)ptr, end = p + count; p < end; ++p)
	{
		*p = value;
	}

	return ptr;
}

int memcmp(const void *ptr1, const void *ptr2, size_t count)
{
	size_t i;
	const uint8_t *p1, *p2;

	p1 = (const uint8_t *)ptr1;
	p2 = (const uint8_t *)ptr2;
	for(i = 0; i < count; ++i)
	{
		if(*p1 != *p2)
		{
			return *p1 - *p2;
		}

		++p1;
		++p2;
	}

	return 0;
}
