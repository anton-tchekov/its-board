#ifndef __STDIO_H__
#define __STDIO_H__

int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list arg);

#endif /* __STDIO_H__ */
