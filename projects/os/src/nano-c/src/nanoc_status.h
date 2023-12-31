/**
 * @file    nanoc_status.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC status codes and error handling
 */

#ifndef __NANOC_STATUS_H__
#define __NANOC_STATUS_H__

#include "nanoc_types.h"

typedef enum
{
	NANOC_STATUS_SUCCESS,
	NANOC_ERROR_UNEXPECTED_TOKEN,
	NANOC_ERROR_EXPECTED_SEMICOLON,
	NANOC_ERROR_EXPECTED_L_PAREN,
	NANOC_ERROR_EXPECTED_R_PAREN,
	NANOC_ERROR_EXPECTED_L_BRACE,
	NANOC_ERROR_EXPECTED_WHILE,
	NANOC_ERROR_EXPECTED_FN,
	NANOC_ERROR_EXPECTED_IDENTIFIER,
	NANOC_ERROR_BREAK_NOT_WITHIN_LOOP_OR_SWITCH,
	NANOC_ERROR_CONTINUE_NOT_WITHIN_LOOP,
	NANOC_ERROR_REDEFINITION,
	NANOC_ERROR_NO_MAIN,
	NANOC_ERROR_UNDERFLOW,
	NANOC_ERROR_OVERFLOW,
	NANOC_ERROR_UNDEFINED_VARIABLE,
	NANOC_ERROR_UNDEFINED_FN,
	NANOC_ERROR_FN_NUM_ARGS,
	NANOC_ERROR_DIVISION_BY_ZERO,
	NANOC_ERROR_INVALID_INSTRUCTION
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
		NanoC_Status __ret; \
		if((__ret = (E))) \
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
		NanoC_Status __ret; \
		if((__ret = (E))) \
		{ \
			return __ret; \
		} \
	} while(0)

#endif /* NANOC_DEBUG */

const char *nanoc_status_message(NanoC_Status status_code);

#endif /* __NANOC_STATUS_H__ */
