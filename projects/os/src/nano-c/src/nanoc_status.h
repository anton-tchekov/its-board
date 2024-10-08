#ifndef __NANOC_STATUS_H__
#define __NANOC_STATUS_H__

#include "nanoc_types.h"

typedef enum
{
	NANOC_STATUS_SUCCESS = 0,
	NANOC_ERROR_UNEXPECTED_TOKEN = -1,
	NANOC_ERROR_EXPECTED_SEMICOLON = -2,
	NANOC_ERROR_EXPECTED_L_PAREN = -3,
	NANOC_ERROR_EXPECTED_R_PAREN = -4,
	NANOC_ERROR_EXPECTED_L_BRACE = -5,
	NANOC_ERROR_EXPECTED_WHILE = -6,
	NANOC_ERROR_EXPECTED_FN = -7,
	NANOC_ERROR_EXPECTED_IDENTIFIER = -8,
	NANOC_ERROR_BREAK_NOT_WITHIN_LOOP_OR_SWITCH = -9,
	NANOC_ERROR_CONTINUE_NOT_WITHIN_LOOP = -10,
	NANOC_ERROR_REDEFINITION = -11,
	NANOC_ERROR_NO_MAIN = -12,
	NANOC_ERROR_UNDERFLOW = -13,
	NANOC_ERROR_OVERFLOW = -14,
	NANOC_ERROR_UNDEFINED = -15,
	NANOC_ERROR_FN_NUM_ARGS = -16,
	NANOC_ERROR_DIVISION_BY_ZERO = -17,
	NANOC_ERROR_INVALID_INSTRUCTION = -18
} NanoC_Status;

#ifdef NANOC_DEBUG

#include <stdio.h>

#define NANOC_THROW(E) \
	do \
	{ \
		NanoC_Status __ret = E; \
		nanoc_log(__FILE__, __func__, __LINE__, __ret); \
		return __ret; \
	} while(0)

#define NANOC_PROPAGATE(E) \
	do \
	{ \
		NanoC_Status __ret = E; \
		if(__ret < 0) \
		{ \
			nanoc_log(__FILE__, __func__, __LINE__, __ret); \
			return __ret; \
		} \
	} while(0)

void nanoc_log(
	const char *file, const char *func, int line, NanoC_Status status_code);

#else /* NANOC_DEBUG */

#define NANOC_THROW(E) \
	do \
	{ \
		return E; \
	} while(0)

#define NANOC_PROPAGATE(E) \
	do \
	{ \
		NanoC_Status __ret = E; \
		if(__ret < 0) \
		{ \
			return __ret; \
		} \
	} while(0)

#endif /* NANOC_DEBUG */

const char *nanoc_status_message(NanoC_Status status_code);

#endif
