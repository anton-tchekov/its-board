#ifndef __NANOC_ERROR_H__
#define __NANOC_ERROR_H__

#include "types.h"

#ifdef NANOC_DEBUG

#include <stdio.h>

#define THROW(e) \
	do \
	{ \
		u8r __ret = e; \
		nanoc_log(__FILE__, __func__, __LINE__, __ret); \
		return __ret; \
	} while(0)

#define PROPAGATE(E) \
	do \
	{ \
		u8r __ret; \
		if((__ret = (E))) \
		{ \
			nanoc_log(__FILE__, __func__, __LINE__, __ret); \
			return __ret; \
		} \
	} while(0)

void nanoc_log(
	const char *file, const char *func, int line, u8r status_code);

#else /* NANOC_DEBUG */

#define THROW(e) \
	do \
	{ \
		return e; \
	} while(0)

#define PROPAGATE(E) \
	do \
	{ \
		u8r __ret; \
		if((__ret = (E))) \
		{ \
			return __ret; \
		} \
	} while(0)

#endif /* NANOC_DEBUG */

typedef enum NANOC_STATUS
{
	NANOC_STATUS_SUCCESS,
	NANOC_ERROR_UNEXPECTED_TOKEN,
	NANOC_ERROR_EXPECTED_SEMICOLON,
	NANOC_ERROR_EXPECTED_L_PAREN,
	NANOC_ERROR_EXPECTED_R_PAREN,
	NANOC_ERROR_EXPECTED_L_BRACE,
	NANOC_ERROR_EXPECTED_WHILE,
	NANOC_ERROR_EXPECTED_IDENTIFIER,
	NANOC_ERROR_BREAK_NOT_WITHIN_LOOP_OR_SWITCH,
	NANOC_ERROR_CONTINUE_NOT_WITHIN_LOOP,
	NANOC_ERROR_VARIABLE_REDEFINITION,
	NANOC_ERROR_STACK_OVERFLOW,
	NANOC_ERROR_DUPLICATE_MAP_ELEMENT,
	NANOC_ERROR_TOO_MANY_VARIABLES,
	NANOC_ERROR_UNDEFINED_VARIABLE,
	NANOC_ERROR_TOO_MANY_FN_ARGS
} NanoC_Status;

const char *nanoc_status_message(u8r status_code);

#endif /* __NANOC_ERROR_H__ */
