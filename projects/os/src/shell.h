/**
 * @file    shell.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   OS Shell
 */

#ifndef __SHELL_H__
#define __SHELL_H__

void shell_init(void);
void shell_open(void);
void shell_key(int key, int c);

void shell_xy(int x, int y);
void shell_cls(void);
void shell_print(const char *s);
void shell_char(int c);
void shell_fg(int color);

#endif /* __SHELL_H__ */
