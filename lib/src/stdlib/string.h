#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t max);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memset(void *ptr, int value, size_t count);

#endif
