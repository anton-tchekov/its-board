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
#include "types.h"
#include "util.h"

#define NANOC_LUT_BIT      0x80
#define NANOC_LUT_ST(X)    ((X) | NANOC_LUT_BIT)
#define NANOC_LUT_LD(X)    ((X) & ~NANOC_LUT_BIT)
#define NANOC_LUT_FOUND(X) ((X) & 0x80)

#endif /* __NANOC_UTIL_H__ */
