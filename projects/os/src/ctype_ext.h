/**
 * @file    ctype_ext.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Extended ctype functionality
 */

#ifndef __CTYPE_EXT_H__
#define __CTYPE_EXT_H__

#include <ctype.h>

static inline int is_identifer_start(int c)
{
	return isalpha(c) || c == '_';
}

static inline int is_identifier_char(int c)
{
	return isalnum(c) || c == '_';
}

static inline int is_binary(int c)
{
	return c == '0' || c == '1';
}

static inline int is_octal(int c)
{
	return c >= '0' && c <= '7';
}

#endif /* __CTYPE_EXT_H__ */
