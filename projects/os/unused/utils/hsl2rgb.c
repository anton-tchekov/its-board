#ifdef OS_UNUSED

#include "hsl2rgb.h"

static f32 hue2rgb(f32 p, f32 q, f32 t)
{
	if(t < 0)
	{
		t += 1;
	}

	if(t > 1)
	{
		t -= 1;
	}

	if(t < 1.0 / 6)
	{
		return p + (q - p) * 6 * t;
	}

	if(t < 1.0 / 2)
	{
		return q;
	}

	if(t < 2.0 / 3)
	{
		return p + (q - p) * (2.0 / 3 - t) * 6;
	}

	return p;
}

u32r hsl2rgb(f32 h, f32 s, f32 l)
{
	u8r r, g, b;
	if(s)
	{
		f32 q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		f32 p = 2 * l - q;
		r = hue2rgb(p, q, h + 1.0 / 3) * 255;
		g = hue2rgb(p, q, h) * 255;
		b = hue2rgb(p, q, h - 1.0 / 3) * 255;
	}
	else
	{
		r = g = b = l;
	}

	return graphics_color(r, g, b);
}

#endif /* OS_UNUSED */
