#ifndef __STRING_H__
#define __STRING_H__

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t max);
char *strcpy(char *dst, const char *src);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t count);

#endif
