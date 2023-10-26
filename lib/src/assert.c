/**
 * @file    assert.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 */

#include "assert.h"
#include "lcd.h"
#include "font.h"
#include "fonts/Terminus16_Bold.h"
#include <stdio.h>

#if ASSERT_EN

void _assert(const char *str, const char *file, int line)
{
	char buf[128];
	sprintf(buf, "%s:%d: Assertion failed:", file, line);
	font_str(10, 10, buf, COLOR_BLACK, COLOR_WHITE, Terminus16_Bold);
	font_str(10, 30, str, COLOR_BLACK, COLOR_WHITE, Terminus16_Bold);
}

#endif /* ASSERT_EN */
