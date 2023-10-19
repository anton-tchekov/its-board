/**
 * @file    nanoc_util.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC utility functions and macros
 */

#ifndef __NANOC_UTIL_H__
#define __NANOC_UTIL_H__

#include <string.h>
#include "nanoc_types.h"

#define NANOC_ARRLEN(a) (sizeof(a) / sizeof(*a))

#define NANOC_LUT_BIT      0x80
#define NANOC_LUT_ST(X)    ((X) | NANOC_LUT_BIT)
#define NANOC_LUT_LD(X)    ((X) & ~NANOC_LUT_BIT)
#define NANOC_LUT_FOUND(X) ((X) & 0x80)

static inline u16r nanoc_read_16(const u8 *p)
{
	return ((u16r)p[0]) | (((u16r)p[1]) << 8);
}

static inline i16r nanoc_read_s16(const u8 *p)
{
	return (i16)nanoc_read_16(p);
}

static inline void nanoc_write_16(u8 *p, u16r v)
{
	p[0] = v;
	p[1] = v >> 8;
}

static inline u32r nanoc_read_32(const u8 *p)
{
	return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static inline void nanoc_write_32(u8 *p, u32r v)
{
	p[0] = v;
	p[1] = v >> 8;
	p[2] = v >> 16;
	p[3] = v >> 24;
}

#endif /* __NANOC_UTIL_H__ */
