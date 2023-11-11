/**
 * @file    nanoc_builtin.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_builtin.h"
#include "nanoc_types.h"
#include "ctype_ext.h"
#include <string.h>

static NanoC_Bool nanoc_builtin_compare(const char *b, const char *t)
{
	while(*b || is_ident(*t))
	{
		if(*b != *t)
		{
			return 0;
		}

		++b;
		++t;
	}

	return 1;
}

size_t nanoc_builtin_find(const NanoC_ParserBuiltins *builtins,
	const char *name)
{
	size_t i;
	for(i = 0; i < builtins->Count; ++i)
	{
		if(nanoc_builtin_compare(builtins->Table[i].Name, name))
		{
			return i + 1;
		}
	}

	return 0;
}
