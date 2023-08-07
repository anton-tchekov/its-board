#include "Terminus16.h"

static const uint8_t _terminus16_bitmap[] =
{
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x00,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x24,
	0x24,
	0x24,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x24,
	0x24,
	0x24,
	0x7E,
	0x24,
	0x24,
	0x7E,
	0x24,
	0x24,
	0x24,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x10,
	0x10,
	0x7C,
	0x92,
	0x90,
	0x90,
	0x7C,
	0x12,
	0x12,
	0x92,
	0x7C,
	0x10,
	0x10,
	0x00,
	0x00,

	0x00,
	0x00,
	0x64,
	0x94,
	0x68,
	0x08,
	0x10,
	0x10,
	0x20,
	0x2C,
	0x52,
	0x4C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x18,
	0x24,
	0x24,
	0x18,
	0x30,
	0x4A,
	0x44,
	0x44,
	0x44,
	0x3A,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x08,
	0x10,
	0x20,
	0x20,
	0x20,
	0x20,
	0x20,
	0x20,
	0x10,
	0x08,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x20,
	0x10,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x10,
	0x20,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x24,
	0x18,
	0x7E,
	0x18,
	0x24,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x7C,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x20,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x04,
	0x04,
	0x08,
	0x08,
	0x10,
	0x10,
	0x20,
	0x20,
	0x40,
	0x40,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x46,
	0x4A,
	0x52,
	0x62,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x08,
	0x18,
	0x28,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x3E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x02,
	0x1C,
	0x02,
	0x02,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x02,
	0x06,
	0x0A,
	0x12,
	0x22,
	0x42,
	0x7E,
	0x02,
	0x02,
	0x02,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7E,
	0x40,
	0x40,
	0x40,
	0x7C,
	0x02,
	0x02,
	0x02,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x1C,
	0x20,
	0x40,
	0x40,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7E,
	0x02,
	0x02,
	0x04,
	0x04,
	0x08,
	0x08,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x02,
	0x02,
	0x04,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x10,
	0x10,
	0x20,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x20,
	0x10,
	0x08,
	0x04,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x7E,
	0x00,
	0x00,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x40,
	0x20,
	0x10,
	0x08,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x04,
	0x08,
	0x08,
	0x00,
	0x08,
	0x08,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7C,
	0x82,
	0x9E,
	0xA2,
	0xA2,
	0xA2,
	0xA6,
	0x9A,
	0x80,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7E,
	0x42,
	0x42,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x40,
	0x40,
	0x40,
	0x40,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x78,
	0x44,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x44,
	0x78,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7E,
	0x40,
	0x40,
	0x40,
	0x78,
	0x40,
	0x40,
	0x40,
	0x40,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7E,
	0x40,
	0x40,
	0x40,
	0x78,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x40,
	0x40,
	0x4E,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7E,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x38,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x0E,
	0x04,
	0x04,
	0x04,
	0x04,
	0x04,
	0x04,
	0x44,
	0x44,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x42,
	0x44,
	0x48,
	0x50,
	0x60,
	0x60,
	0x50,
	0x48,
	0x44,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x82,
	0xC6,
	0xAA,
	0x92,
	0x92,
	0x82,
	0x82,
	0x82,
	0x82,
	0x82,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x62,
	0x52,
	0x4A,
	0x46,
	0x42,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7C,
	0x40,
	0x40,
	0x40,
	0x40,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x4A,
	0x3C,
	0x02,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7C,
	0x50,
	0x48,
	0x44,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x3C,
	0x42,
	0x40,
	0x40,
	0x3C,
	0x02,
	0x02,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0xFE,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x24,
	0x24,
	0x24,
	0x18,
	0x18,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x82,
	0x82,
	0x82,
	0x82,
	0x82,
	0x92,
	0x92,
	0xAA,
	0xC6,
	0x82,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x42,
	0x42,
	0x24,
	0x24,
	0x18,
	0x18,
	0x24,
	0x24,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x82,
	0x82,
	0x44,
	0x44,
	0x28,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x7E,
	0x02,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x40,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x38,
	0x20,
	0x20,
	0x20,
	0x20,
	0x20,
	0x20,
	0x20,
	0x20,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x40,
	0x40,
	0x20,
	0x20,
	0x10,
	0x10,
	0x08,
	0x08,
	0x04,
	0x04,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x38,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x08,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x10,
	0x28,
	0x44,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x7E,
	0x00,
	0x00,

	0x10,
	0x08,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3C,
	0x02,
	0x3E,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x40,
	0x40,
	0x40,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3C,
	0x42,
	0x40,
	0x40,
	0x40,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x02,
	0x02,
	0x02,
	0x3E,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x7E,
	0x40,
	0x40,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x0E,
	0x10,
	0x10,
	0x7C,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3E,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x02,
	0x02,
	0x3C,
	0x00,

	0x00,
	0x00,
	0x40,
	0x40,
	0x40,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x10,
	0x10,
	0x00,
	0x30,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x04,
	0x04,
	0x00,
	0x0C,
	0x04,
	0x04,
	0x04,
	0x04,
	0x04,
	0x04,
	0x44,
	0x44,
	0x38,
	0x00,

	0x00,
	0x00,
	0x40,
	0x40,
	0x40,
	0x42,
	0x44,
	0x48,
	0x70,
	0x48,
	0x44,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x30,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x38,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0xFC,
	0x92,
	0x92,
	0x92,
	0x92,
	0x92,
	0x92,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x7C,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x7C,
	0x40,
	0x40,
	0x40,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3E,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x02,
	0x02,
	0x02,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x5E,
	0x60,
	0x40,
	0x40,
	0x40,
	0x40,
	0x40,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x3E,
	0x40,
	0x40,
	0x3C,
	0x02,
	0x02,
	0x7C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x10,
	0x10,
	0x10,
	0x7C,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x0E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x24,
	0x24,
	0x18,
	0x18,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x82,
	0x82,
	0x92,
	0x92,
	0x92,
	0x92,
	0x7C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x42,
	0x42,
	0x24,
	0x18,
	0x24,
	0x42,
	0x42,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x42,
	0x3E,
	0x02,
	0x02,
	0x3C,
	0x00,

	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x7E,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x7E,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x0C,
	0x10,
	0x10,
	0x10,
	0x20,
	0x10,
	0x10,
	0x10,
	0x10,
	0x0C,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x00,
	0x30,
	0x08,
	0x08,
	0x08,
	0x04,
	0x08,
	0x08,
	0x08,
	0x08,
	0x30,
	0x00,
	0x00,
	0x00,
	0x00,

	0x00,
	0x62,
	0x92,
	0x8C,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
};

const Font _Terminus16 =
{
	.Width = 8,
	.Height = 16,
	.Bitmap = _terminus16_bitmap,
	.Extra = UC("ä")
};

const Font *const Terminus16 = &_Terminus16;
