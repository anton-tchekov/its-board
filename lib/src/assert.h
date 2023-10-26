/**
 * @file    assert.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 * @brief   Assertions
 */

#ifndef __ASSERT_H__
#define __ASSERT_H__

#if ASSERT_EN

#define assert(_COND) \
	if(!(_COND)) \
	{ \
		_assert(#_COND, __FILE__, __LINE__); \
	}

void _assert(const char *str, const char *file, int line);

#else /* ASSERT_EN */

#define assert(_COND)

#endif /* ASSERT_EN */

#endif /* __ASSERT_H__ */
