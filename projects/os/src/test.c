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
#include <stdio.h>

void test_open(void)
{
	shell_cls();
	shell_print("--- [ TEST MODE ] ---\n\n");

	char pri[64];
	u8 buf[512];
	int retval;
	FATFS fs;

	retval = f_mkfs("0:", 0, buf, sizeof(buf));
	sprintf(pri, "RAMDISK MKFS -> %d\n", retval);
	shell_print(pri);

	retval = f_mount(&fs, "0:", 1);
	sprintf(pri, "MOUNT -> %d\n", retval);
	shell_print(pri);

	retval = f_unmount("0:");
	sprintf(pri, "UNMOUNT -> %d\n", retval);
	shell_print(pri);
}

void test_key(int key, int c)
{
	(void)key, (void)c;
}
