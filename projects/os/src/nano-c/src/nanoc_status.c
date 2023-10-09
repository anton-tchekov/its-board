/**
 * @file    nanoc_status.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_status.h"
#include "nanoc_debug.h"
#include "nanoc_util.h"

#ifdef NANOC_DEBUG

void nanoc_log(
	const char *file, const char *func, int line, NanoC_Status status_code)
{
	fprintf(stderr, "%s:%d (%s) - %s\n",
		file, line, func, nanoc_status_message(status_code));
}

#endif /* NANOC_DEBUG */

const char *nanoc_status_message(NanoC_Status status_code)
{
	static const char *err_msgs[] =
	{
		"SUCCESS",
		"UNEXPECTED_TOKEN",
		"EXPECTED_SEMICOLON",
		"EXPECTED_L_PAREN",
		"EXPECTED_R_PAREN",
		"EXPECTED_L_BRACE",
		"EXPECTED_WHILE",
		"EXPECTED_IDENTIFIER",
		"BREAK_NOT_WITHIN_LOOP_OR_SWITCH",
		"CONTINUE_NOT_WITHIN_LOOP",
		"VARIABLE_REDEFINITION",
		"STACK_UNDERFLOW",
		"STACK_OVERFLOW",
		"DUPLICATE_MAP_ELEMENT",
		"TOO_MANY_VARIABLES",
		"UNDEFINED_VARIABLE",
		"UNDEFINED_FN",
		"FN_NUM_ARGS",
		"DIVISION_BY_ZERO",
		"INVALID_INSTRUCTION"
	};

	nanoc_assert(status_code < ARRLEN(err_msgs));
	return err_msgs[status_code];
}
