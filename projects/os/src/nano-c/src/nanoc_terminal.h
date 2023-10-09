/**
 * @file    nanoc_terminal.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC terminal escape sequences
 */

#ifndef __NANOC_TERMINAL_H__
#define __NANOC_TERMINAL_H__

#ifdef NANOC_DESKTOP

#define NANOC_TERMINAL_BOLD_WHITE "\033[0;1m"
#define NANOC_TERMINAL_BOLD_RED   "\033[31;1m"
#define NANOC_TERMINAL_RESET      "\033[0m"

#endif /* NANOC_DESKTOP */

#endif /* __NANOC_TERMINAL_H__ */
