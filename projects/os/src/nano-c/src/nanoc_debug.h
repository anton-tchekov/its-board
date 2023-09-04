#ifndef __NANOC_DEBUG_H__
#define __NANOC_DEBUG_H__

#ifdef NANOC_DEBUG

#include "nanoc_terminal.h"

#define nanoc_unreachable() \
	do \
	{ \
		nanoc_fatal_error("Code marked as unreachable was executed", \
			__FILE__, __func__, __LINE__); \
	} \
	while(0)

#define nanoc_assert(expr) \
	do \
	{ \
		if(!(expr)) \
		{ \
			nanoc_fatal_error("Assertion failed: `" \
				COLOR_BOLD_WHITE #expr COLOR_RESET "`", \
				__FILE__, __func__, __LINE__); \
		} \
	} \
	while(0)

#define nanoc_unimplemented() \
	do \
	{ \
		nanoc_fatal_error("Operation marked as not implemented was executed", \
			__FILE__, __func__, __LINE__); \
	} \
	while(0)

void nanoc_fatal_error(const char *msg, const char *file,
	const char *func, int line);

#else /* NANOC_DEBUG */

#include "panic.h"

#define nanoc_unreachable() panic()
#define nanoc_assert(expr) if(!(expr)) { panic(); }
#define nanoc_unimplemented() panic()

#endif /* NANOC_DEBUG */

#endif /* __NANOC_DEBUG_H__ */
