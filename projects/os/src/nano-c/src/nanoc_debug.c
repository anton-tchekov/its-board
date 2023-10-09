/**
 * @file    nanoc_debug.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_debug.h"
#include "nanoc_terminal.h"

#ifdef NANOC_DEBUG

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <signal.h>
#endif /* __linux__ */

void nanoc_fatal_error(const char *msg, const char *file,
	const char *func, int line)
{
	fprintf(stderr,
		"\n"
		"----------------------------\n"
		"--- " NANOC_TERMINAL_BOLD_RED "[NANO-C] FATAL ERROR" NANOC_TERMINAL_RESET " ---\n"
		"----------------------------\n"
		"\n"
		NANOC_TERMINAL_BOLD_WHITE "%s:%d" NANOC_TERMINAL_RESET
		" in function `" NANOC_TERMINAL_BOLD_WHITE "%s" NANOC_TERMINAL_RESET "`:\n"
		"%s\n\n",
		file, line, func, msg);

#ifdef __linux__
	raise(SIGTRAP);
#endif /* __linux__ */

	exit(1);
}

#endif /* NANOC_DEBUG */
