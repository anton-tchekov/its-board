#ifndef __NANOC_CTYPE_H__
#define __NANOC_CTYPE_H__

#include "types.h"

u32r nanoc_isupper(u32 *p);
u32r nanoc_islower(u32 *p);
u32r nanoc_isdigit(u32 *p);
u32r nanoc_isalpha(u32 *p);
u32r nanoc_isalnum(u32 *p);
u32r nanoc_iscntrl(u32 *p);
u32r nanoc_isgraph(u32 *p);
u32r nanoc_isprint(u32 *p);
u32r nanoc_ispunct(u32 *p);
u32r nanoc_isspace(u32 *p);
u32r nanoc_isxdigit(u32 *p);
u32r nanoc_tolower(u32 *p);
u32r nanoc_toupper(u32 *p);

#endif /* __NANOC_CTYPE_H__ */
