#ifdef NANOC_DESKTOP

#include "file_io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_READ_CHUNK (16 * 1024)

int file_put_contents(
	const char *filename, int mode, const void *content, size_t length)
{
	FILE *fp;
	if(!(fp = fopen(filename, (mode == FILE_IO_MODE_BINARY) ? "wb" : "w")))
	{
		return 1;
	}

	if(fwrite(content, 1, length, fp) != length)
	{
		fclose(fp);
		return 1;
	}

	fclose(fp);
	return 0;
}

void *file_get_contents(
	const char *filename, int mode, size_t *length)
{
	size_t cur_length, cur_capacity, bytes_read;
	uint8_t *content, *resized_content;
	FILE *fp;

	/* Open file */
	if(!(fp = fopen(filename, (mode == FILE_IO_MODE_BINARY) ? "rb" : "r")))
	{
		return NULL;
	}

	/* Initialize buffer */
	cur_length = 0;
	cur_capacity = FILE_READ_CHUNK;
	if(!(content = malloc(cur_capacity)))
	{
		fclose(fp);
		return NULL;
	}

	/* Loop until the number of read bytes is less than the chunk size */
	do
	{
		if((cur_length + FILE_READ_CHUNK) > cur_capacity)
		{
			/* Double buffer size when full */
			cur_capacity <<= 1;
			if(!(resized_content = realloc(content, cur_capacity)))
			{
				free(content);
				fclose(fp);
				return NULL;
			}

			content = resized_content;
		}

		/* Read next chunk */
		bytes_read = fread(content + cur_length, 1, FILE_READ_CHUNK, fp);
		cur_length += bytes_read;
	}
	while(bytes_read == FILE_READ_CHUNK);

	fclose(fp);
	if(mode == FILE_IO_MODE_TEXT)
	{
		/* Make space for null terminator */
		++cur_length;
	}

	/* Resize buffer to perfectly fit contents */
	if(!(resized_content = realloc(content, cur_length)))
	{
		free(content);
		return NULL;
	}

	content = resized_content;
	if(mode == FILE_IO_MODE_TEXT)
	{
		/* Append null terminator */
		content[cur_length - 1] = '\0';
	}

	*length = cur_length;
	return content;
}

#endif /* NANOC_DESKTOP */
