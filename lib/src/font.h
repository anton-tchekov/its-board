#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>
#include "utf8.h"

typedef struct
{
	int Width;
	int Height;
	const uint8_t *Bitmap;
	int Extra;
} Font;

void font_char(int x, int y, int c, int fg, int bg, const Font *font);
void font_str(int x, int y, const char *s, int fg, int bg, const Font *font);

#endif /* __FONT_H__ */
