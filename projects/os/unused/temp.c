#if 0

#define PIN 4

// Variante B
int draw_chessboard(int x, int y, void *handle)
{
	if(((x / 8) & 1) == ((y / 8) & 1))
	{
		return COLOR_WHITE;
	}

	return COLOR_BLACK;

	(void)handle;
}


	int cx = 30;


	font_str(100, 10, "Hello World\xFF\xFF\xFF", COLOR_WHITE, COLOR_BLACK, Terminus16);
	font_str(100, 30, "Hello World Bold\xFF\xFF", COLOR_WHITE, COLOR_BLACK, Terminus16_Bold);

	// Variante A - Explizit
	{
		int x, y;
		int w = LCD_WIDTH - 100;
		int h = LCD_HEIGHT - 100;
		lcd_window_start(50, 50, w, h);
		for(y = 0; y < h; ++y)
		{
			for(x = 0; x < w; ++x)
			{
				lcd_emit(lcd_color(x * 255 / w, y * 255 / h, 0));
			}
		}
		lcd_window_end();
	}

	uint32_t t0 = timer_get();

	// Variante B - Mit Callback
	lcd_callback(LCD_WIDTH / 2 - 40, LCD_HEIGHT / 2 - 40, 80, 80,
		NULL, draw_chessboard);

	uint32_t t1 = timer_get();
	char buf[32];

	font_str(10, 80, "Time Delta to draw Chessboard:",
		COLOR_BLACK, COLOR_WHITE, Terminus16);

	timer_delta_str(buf, t1 - t0);

	font_str(50, 100, buf,
		COLOR_BLACK, COLOR_WHITE, Terminus16);


/*	draw_fancy_logo(LCD_WIDTH / 2 - LOGO_WIDTH / 2,
		LCD_HEIGHT / 2 - LOGO_HEIGHT / 2,
		LOGO_WIDTH, LOGO_HEIGHT, 0);*/


#define LOGO_WIDTH   160
#define LOGO_HEIGHT  200


#include <stdint.h>
#include "lcd.h"

#define RADIUS    7
#define THICKNESS 3

typedef struct
{
	int16_t X, Y;
} Point;

typedef struct
{
	int Width, Height;
	uint8_t *Pixels;
} Bitmap;

static const Point _poslut[360] =
{
	{  60,  30 },
	{  60, -30 },
	{ -60, -30 },
	{ -60,  30 },
};

static int min(int a, int b)
{
	return a < b ? a : b;
}

static int max(int a, int b)
{
	return a > b ? a : b;
}

static void bitmap_clear(Bitmap *a)
{
	int i, size = a->Width * a->Height;
	for(i = 0; i < size; ++i)
	{
		a->Pixels[i] = 0;
	}
}

static int getpixel(Bitmap *a, int x, int y)
{
	return a->Pixels[y * a->Width + x];
}

static void setpixel(Bitmap *a, int x, int y, int val)
{
	a->Pixels[y * a->Width + x] = val;
}

static int havg(Bitmap *a, int x, int y, int r)
{
	int sx, ex, avg;

	sx = x - r;
	if(sx < 0) { sx = 0; }

	ex = x + r;
	if(ex >= a->Width) { ex = a->Width - 1; }

	avg = 0;
	for(x = sx; x <= ex; ++x)
	{
		avg += getpixel(a, x, y);
	}

	return avg / (ex - sx + 1);
}

static int vavg(Bitmap *a, int x, int y, int r)
{
	int sy, ey, avg, cnt;

	sy = y - r;
	if(sy < 0) { sy = 0; }

	ey = y + r;
	if(ey >= a->Height) { ey = a->Height - 1; }

	avg = 0;
	for(y = sy; y <= ey; ++y)
	{
		avg += getpixel(a, x, y);
	}

	return avg / (ey - sy + 1);
}

static void vblur(Bitmap *a, Bitmap *b)
{
	int x, y;
	for(y = 0; y < a->Height; ++y)
	{
		for(x = 0; x < a->Width; ++x)
		{
			setpixel(b, x, y, vavg(a, x, y, RADIUS));
		}
	}
}

static int bloom_blit_fn(int x, int y, void *handle)
{
	Bitmap **both = handle;
	Bitmap *a = both[0];
	Bitmap *b = both[1];

	int acolor = getpixel(a, x, y);
	int bcolor = havg(b, x, y, RADIUS);

	return lcd_color(bcolor, 0, 0);
	//lcd_color(min(acolor + bcolor, 0xFF), acolor, acolor);
}

static void bloom_n_blit(Bitmap *a, Bitmap *b, int x, int y)
{
	Bitmap *both[2] = { a, b };
	vblur(a, b);
	lcd_callback(x, y, a->Width, a->Height, both, bloom_blit_fn);
}

static int blit_fn(int x, int y, void *handle)
{
	Bitmap *a = handle;
	int acolor = getpixel(a, x, y);
	return lcd_color(acolor, acolor, acolor);
}

static void blit(Bitmap *a, int x, int y)
{
	lcd_callback(x, y, a->Width, a->Height, a, blit_fn);
}

static void genkernel(Bitmap *a, int r)
{
	int x, y, size;
	a->Height = a->Width = size = 2 * r + 1;

	for(y = 0; y < size; ++y)
	{
		for(x = 0; x < size; ++x)
		{
			int dx = x - r;
			int dy = y - r;
			int d = dx * dx + dy * dy;
			setpixel(a, x, y, 0xFF - d * 10);
		}
	}
}

static void pixel(Bitmap *a, Bitmap *kernel, int x, int y)
{
	int p, q;
	int r = (kernel->Width - 1) / 2;
	for(p = 0; p < kernel->Height; ++p)
	{
		for(q = 0; q < kernel->Width; ++q)
		{
			int ox, oy;
			ox = q - r;
			oy = p - r;

			setpixel(a, x - ox, y - oy,
				max(getpixel(a, x - ox, y - oy),
					getpixel(kernel, p, q)));
		}
	}
}

static int abs(int v)
{
	return v < 0 ? -v : v;
}

static void line(Bitmap *a, Bitmap *kernel, Point p0, Point p1)
{
	int x0 = p0.X;
	int y0 = p0.Y;
	int x1 = p1.X;
	int y1 = p1.Y;

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	while(x0 != x1 && y0 != y1)
	{
		pixel(a, kernel, x0, y0);

		e2 = err;

		if(e2 > -dx)
		{
			err -= dy;
			x0 += sx;
		}

		if(e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void draw_fancy_logo(int x, int y, int w, int h, int deg)
{
	int i, size = w * h;
	uint8_t apixels[size], bpixels[size],
		kpixels[(2 * THICKNESS + 1) * (2 * THICKNESS + 1)];
	Point top = { w / 2, RADIUS };
	Point bottom = { w / 2, h - RADIUS };
	Point center[4];
	Bitmap kernel;
	Bitmap a =
	{
		.Width = w,
		.Height = h,
		.Pixels = apixels
	};

	Bitmap b =
	{
		.Width = w,
		.Height = h,
		.Pixels = bpixels
	};

	kernel.Pixels = kpixels;
	genkernel(&kernel, THICKNESS);

	bitmap_clear(&a);

	for(i = 0; i < 4; ++i)
	{
		//Point p = _poslut[(deg + 90 * i) % 360];
		Point p = _poslut[i];
		center[i].X = w / 2 + p.X;
		center[i].Y = h / 2 + p.Y;
	}

	line(&a, &kernel, (Point) { 25, 25 }, (Point) { w - 25, h-25 });
	line(&a, &kernel, (Point) { w-25, 25 }, (Point) { 25, h-25 });

	/*for(i = 0; i < 4; ++i)
	{
		line(&a, &kernel, top, center[i]);
		line(&a, &kernel, bottom, center[i]);
		line(&a, &kernel, center[i], center[(i + 1) & 3]);
	}*/

	bloom_n_blit(&a, &b, x, y);
	//bloom_n_blit(&kernel, NULL, x, y);


	blit(&a, x + 160, y);
}


#endif


