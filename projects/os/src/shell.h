#ifndef __SHELL_H__
#define __SHELL_H__

void shell_init(void);
void shell_open(void);
void shell_key(int key, int c);

void shell_cls(void);
void shell_print(const char *s);
void shell_char(int c);
void shell_fg(int color);

#endif /* __SHELL_H__ */
