/**
 * @file    nanoc_builtin.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_builtin.h"
#include "nanoc_types.h"
#include <string.h>

static NanoC_Bool _match(const char *builtin, const char *token, size_t len)
{
	return strlen(builtin) == len && !strncmp(builtin, token, len);
}

NanoC_Bool nanoc_builtin_find(const NanoC_ParserBuiltins *builtins,
	const char *name, size_t len, size_t *idx)
{
	size_t i, count;
	const NanoC_ParserBuiltin *elem;

	elem = builtins->Table;
	count = builtins->Count;
	for(i = 0; i < count; ++i, ++elem)
	{
		if(_match(elem->Name, name, len))
		{
			*idx = i;
			return 1;
		}
	}

	return 0;
}
