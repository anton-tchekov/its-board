#ifndef __UTIL_H__
#define __UTIL_H__

#include "types.h"

#define ARRLEN(a) (sizeof(a) / sizeof(*a))

static inline u16r read_16(const u8 *p)
{
	return ((u16r)p[0]) | (((u16r)p[1]) << 8);
}

static inline i16r read_s16(const u8 *p)
{
	return (i16)read_16(p);
}

static inline void write_16(u8 *p, u16r v)
{
	p[0] = v;
	p[1] = v >> 8;
}

static inline u32r read_32(const u8 *p)
{
	return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

static inline void write_32(u8 *p, u32r v)
{
	p[0] = v;
	p[1] = v >> 8;
	p[2] = v >> 16;
	p[3] = v >> 24;
}

#endif /* __UTIL_H__ */
