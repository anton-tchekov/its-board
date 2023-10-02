#ifdef FSTEST

#include "ff.h"
#include "../../ramdisk/ramdisk.h"
#include <stdio.h>

int main(void)
{
	u8 buf[512];
	int retval;
	FATFS fs;

	retval = f_mkfs("0:", 0, buf, sizeof(buf));
	printf("RAMDISK MKFS -> %d\n", retval);

	retval = f_mount(&fs, "0:", 1);
	printf("MOUNT -> %d\n", retval);

	retval = f_unmount("0:");
	printf("UNMOUNT -> %d\n", retval);

	return 0;
}

#endif
