#ifdef ENABLE_RAMDISK

#ifndef __RAMDISK_H__
#define __RAMDISK_H__

#include "types.h"

void ramdisk_write(const u8 *buf, size_t sector, size_t count);
void ramdisk_read(u8 *buf, size_t sector, size_t count);
void ramdisk_info_print(void);

#endif /* __RAMDISK_H__ */

#endif
