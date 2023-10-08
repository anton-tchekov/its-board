/**
 * @file    test.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "test.h"
#include "ff.h"
#include "shell.h"
#include "types.h"
#include "sd/sd.h"
#include <stdio.h>

void test_open(void)
{
	char buf[32];
	SD sd;

	shell_cls();
	shell_print("--- [ TEST MODE ] ---\n\n");

	sprintf(buf, "status = %d\n", sd_init(&sd));
	shell_print(buf);
	sd_info_print(&sd);
}

void test_key(int key, int c)
{
	(void)key, (void)c;
}
