/**
 * @file    nanoc_ctype.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_ctype.h"
#include <ctype.h>

i32r nanoc_isupper(i32r a, i32 *p)
{
	return isupper(p[0]);
	(void)a;
}

i32r nanoc_islower(i32r a, i32 *p)
{
	return islower(p[0]);
	(void)a;
}

i32r nanoc_isdigit(i32r a, i32 *p)
{
	return isdigit(p[0]);
	(void)a;
}

i32r nanoc_isalpha(i32r a, i32 *p)
{
	return isalpha(p[0]);
	(void)a;
}

i32r nanoc_isalnum(i32r a, i32 *p)
{
	return isalnum(p[0]);
	(void)a;
}

i32r nanoc_isprint(i32r a, i32 *p)
{
	return isprint(p[0]);
	(void)a;
}

i32r nanoc_isspace(i32r a, i32 *p)
{
	return isspace(p[0]);
	(void)a;
}

i32r nanoc_isxdigit(i32r a, i32 *p)
{
	return isxdigit(p[0]);
	(void)a;
}

i32r nanoc_tolower(i32r a, i32 *p)
{
	return tolower(p[0]);
	(void)a;
}

i32r nanoc_toupper(i32r a, i32 *p)
{
	return toupper(p[0]);
	(void)a;
}
