/**
 * @file    types.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Shorthand types
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ARRLEN(a) (sizeof(a) / sizeof(*a))

#define PROPAGATE(E) \
	do \
	{ \
		int __ret = (E); \
		if(__ret) \
		{ \
			return __ret; \
		} \
	} while(0)

typedef uint32_t u8r;
typedef uint8_t u8;

typedef uint32_t u16r;
typedef uint16_t u16;

typedef uint32_t u32r;
typedef uint32_t u32;

typedef int32_t i8r;
typedef int8_t i8;

typedef int32_t i16r;
typedef int16_t i16;

typedef int32_t i32r;
typedef int32_t i32;

typedef float f32;
typedef double f64;

#endif
