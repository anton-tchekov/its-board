#ifndef __NANOC_CTYPE_H__
#define __NANOC_CTYPE_H__

#include "types.h"

i32r nanoc_isupper(i32r a, i32 *p);
i32r nanoc_islower(i32r a, i32 *p);
i32r nanoc_isdigit(i32r a, i32 *p);
i32r nanoc_isalpha(i32r a, i32 *p);
i32r nanoc_isalnum(i32r a, i32 *p);
i32r nanoc_isprint(i32r a, i32 *p);
i32r nanoc_isspace(i32r a, i32 *p);
i32r nanoc_isxdigit(i32r a, i32 *p);
i32r nanoc_tolower(i32r a, i32 *p);
i32r nanoc_toupper(i32r a, i32 *p);

#endif /* __NANOC_CTYPE_H__ */
