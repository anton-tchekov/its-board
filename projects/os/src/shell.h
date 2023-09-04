#ifndef __SHELL_H__
#define __SHELL_H__

void shell_init(void);
void shell_update(void);
void shell_print(const char *s);
void shell_char(int c);
void shell_fg(int color);

#endif /* __SHELL_H__ */
