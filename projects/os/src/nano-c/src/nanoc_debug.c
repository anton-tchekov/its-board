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
		"--- " COLOR_BOLD_RED "[NANO-C] FATAL ERROR" COLOR_RESET " ---\n"
		"----------------------------\n"
		"\n"
		COLOR_BOLD_WHITE "%s:%d" COLOR_RESET
		" in function `" COLOR_BOLD_WHITE "%s" COLOR_RESET "`:\n"
		"%s\n\n",
		file, line, func, msg);

#ifdef __linux__
	raise(SIGTRAP);
#endif /* __linux__ */

	exit(1);
}

#endif /* NANOC_DEBUG */
