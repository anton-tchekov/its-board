#include "ramdisk.h"
#include <string.h>

static u8 ramdisk[512 * 192];

void ramdisk_write(const u8 *buf, size_t sector, size_t count)
{
	memcpy(ramdisk + (sector << 9), buf, count << 9);
}

void ramdisk_read(u8 *buf, size_t sector, size_t count)
{
	memcpy(buf, ramdisk + (sector << 9), count << 9);
}
