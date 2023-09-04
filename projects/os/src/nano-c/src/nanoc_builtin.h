#ifndef __NANOC_BUILTIN_H__
#define __NANOC_BUILTIN_H__

#include "types.h"

typedef i32r (*NanoC_Builtin)(i32 *);

typedef struct NANOC_BUILTINS
{
	size_t Count;
	i32r (**Functions)(i32r, i32 *);
} NanoC_Builtins;

#endif /* __NANOC_BUILTIN_H__ */
