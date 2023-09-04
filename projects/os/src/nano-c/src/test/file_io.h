#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#ifdef NANOC_DESKTOP

#include <stddef.h>

enum FILE_IO_MODE
{
	FILE_IO_MODE_BINARY,
	FILE_IO_MODE_TEXT
};

int file_put_contents(
	const char *filename, int mode, const void *content, size_t length);

void *file_get_contents(
	const char *filename, int mode, size_t *length);

#endif /* NANOC_DESKTOP */

#endif /* __FILE_IO_H__ */
