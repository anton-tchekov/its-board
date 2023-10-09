/**
 * @file    nanoc_builtin.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC builtin function interface
 */

#ifndef __NANOC_BUILTIN_H__
#define __NANOC_BUILTIN_H__

#include "types.h"

typedef i32r (*NanoC_Builtin)(i32r, i32 *);

typedef struct NANOC_BUILTINS
{
	size_t Count;
	const NanoC_Builtin *Functions;
} NanoC_Builtins;

typedef struct NANOC_PARSER_BUILTIN
{
	const char *Name;
	u8 NumArgs;
	u8 IsVariadic;
} NanoC_ParserBuiltin;

typedef struct NANOC_PARSER_BUILTINS
{
	size_t Count;
	const NanoC_ParserBuiltin *Table;
} NanoC_ParserBuiltins;

int nanoc_builtin_find(const NanoC_ParserBuiltins *builtins,
	const char *name, size_t len);

#endif /* __NANOC_BUILTIN_H__ */
