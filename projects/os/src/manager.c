#include "manager.h"
#include "keyboard.h"
#include "terminal.h"
#include <string.h>

#define FILE_OFFSET  1
#define EXT_OFFSET  14

#define MAX_ENTRIES       (SCREEN_HEIGHT - 3)

//terminal_set(x, y, buf[i]);

typedef struct
{
	i16 OffsetX;
	i16 Selected;
	i16 Count;
	i16 Offset;
	char Path[64];
} Side;

// static uint8_t _off_xs[2] = { 0, SCREEN_WIDTH / 2 };

static Side _sides[2];
static int _side;

static void printc(int x, int y, int c)
{
	terminal_set(x, y, c | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
}

static void prints(int x, int y, const char *s)
{
	int c;
	while((c = *s++))
	{
		terminal_set(x++, y, c | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
	}
}

void manager_init(void)
{
	strcpy(_sides[0].Path, "/");
	strcpy(_sides[1].Path, "/");
}

static void manager_status_side(u8r side)
{
#if 0
	uint8_t i, n;
	char c, *s, buf[16];

	vga_color(TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
	vga_position(_off_xs[side] + 3, 0);

	/* Top Line */
	for(i = 3, s = _paths[side]; (c = *s); ++s, ++i)
	{
		uart_tx(c);
	}

	uart_tx(' ');
	for(; i < SCREEN_WIDTH / 2 - 3; ++i)
	{
		uart_tx('-');
	}

	/* Bottom Line */
	n = snprintf(buf, sizeof(buf), "%d", _counts[side]);

	vga_position(_off_xs[side] + 2, SCREEN_HEIGHT - 2);
	for(i = 2; i < SCREEN_WIDTH / 2 - 4 - n; ++i)
	{
		uart_tx('-');
	}

	uart_tx(' ');
	uart_tx_str(buf);
#endif
}

static void manager_list_side(u8r side)
{
#if 0
	char buf[FILE_BYTES];
	int16_t i;
	uint8_t j, u, x;

	vga_color(FG_WHITE | BG_BLACK);
	vga_clear(_off_xs[side] + 1, 1, SCREEN_WIDTH / 2 - 2, MAX_ENTRIES);

	for(u = 1, i = _offsets[side]; i < _offsets[side] + MAX_ENTRIES; ++i, ++u)
	{
		x = 1;
		if(i >= _counts[side])
		{
			break;
		}

		if(side == _side && i == _sels[_side])
		{
			vga_color(FG_BLACK | BG_WHITE);
		}
		else
		{
			vga_color(FG_WHITE | BG_BLACK);
		}

		vga_position(_off_xs[side] + FILE_OFFSET, u);
		uart_tx(' ');
		xram_read(_xram_offs[side] + (u32)i * FILE_BYTES, buf, FILE_BYTES);

		for(j = 1; j < 9 && buf[j] && buf[j] != '.'; ++j, ++x)
		{
			uart_tx(buf[j]);
		}

		if(buf[0] == 'D')
		{
			uart_tx('/');
			++x;
		}

		while(x < EXT_OFFSET)
		{
			uart_tx(' ');
			++x;
		}

		if(buf[0] == 'D')
		{
			uart_tx_str("DIR");
			x += 3;
		}

		if(buf[j])
		{
			++j;
			for(; j < 13 && buf[j]; ++j, ++x)
			{
				uart_tx(buf[j]);
			}
		}

		if(side == _side)
		{
			while(x < 38)
			{
				uart_tx(' ');
				++x;
			}
		}
	}
#endif
}

static void _help_line(void)
{
	static const char help[] =
		"F5 Copy | F6 Move/Rename | F7 MkDir | F8 Delete";

	prints(TERMINAL_W / 2 - sizeof(help) / 2, TERMINAL_H - 1, help);
}

static void _border_vertical(void)
{
	u8r i;
	for(i = 1; i < TERMINAL_H - 2; ++i)
	{
		printc(0, i, '|');
		printc(TERMINAL_W / 2 - 1, i, '|');
		printc(TERMINAL_W / 2, i, '|');
		printc(TERMINAL_W - 1, i, '|');
	}
}

static void _border_horizontal(void)
{
	prints(0, 0, "+-");
	prints(0, TERMINAL_H - 2, "+-");

	prints(TERMINAL_W / 2 - 2, 0, "-++-");
	prints(TERMINAL_W / 2 - 2, TERMINAL_H - 2, "-++-");

	prints(TERMINAL_W - 2, 0, "-+");
	prints(TERMINAL_W - 2, TERMINAL_H - 2, "-+");
}

static void _filler(void)
{
	_border_horizontal();
	_border_vertical();
	_help_line();
}

void manager_open(void)
{
#if 0
	if((_counts[0] = folder_list(_paths[0], _xram_offs[0])) < 0)
	{
		return;
	}

	if((_counts[1] = folder_list(_paths[1], _xram_offs[1])) < 0)
	{
		return;
	}

	manager_help_line();
	manager_sides();
	manager_list_side(0);
	manager_list_side(1);
	manager_status_side(0);
	manager_status_side(1);
#endif

	_filler();
}

#if 0
static void manager_home(void)
{
	Side *side = &_sides[_side];
	side->Selected = 0;
	side->Offset = 0;
	manager_list_side(_side);
}

static void manager_end(void)
{
	Side *side = &_sides[_side];
	side->Selected = side->Count - 1;
	side->Offset = side->Selected - MAX_ENTRIES - 1;
	if(side->Offset < 0)
	{
		side->Offset = 0;
	}

	manager_list_side(_side);
}

static void manager_up(void)
{
	--_sels[_side];
	if(_sels[_side] < 0)
	{
		_sels[_side] = 0;
	}

	if(_sels[_side] < _offsets[_side])
	{
		_offsets[_side] = _sels[_side];
	}

	manager_list_side(_side);
}

static void manager_down(void)
{
	++_sels[_side];
	if(_sels[_side] > _counts[_side] - 1)
	{
		_sels[_side] = _counts[_side] - 1;
	}

	if(_sels[_side] > _offsets[_side] + MAX_ENTRIES - 1)
	{
		_offsets[_side] = _offsets[_side] + MAX_ENTRIES - 1;
	}

	manager_list_side(_side);
}

static void manager_enter(void)
{
	uint8_t len = strlen(_paths[_side]);
	if(len < 2)
	{
		return;
	}

	char *p = _paths[_side] + len - 2;

	for(; *p != '/'; --p) ;
	++p;
	*p = '\0';

	_counts[_side] = folder_list(_paths[_side], _xram_offs[_side]);
	manager_status_side(_side);
	manager_list_side(_side);




	char buf[FILE_BYTES], *p;
	xram_read(_xram_offs[_side] + (u32)_sels[_side] * FILE_BYTES, buf, FILE_BYTES);
	if(buf[0] != 'D')
	{
		break;
	}

	p = _paths[_side] + strlen(_paths[_side]);
	strcpy(p, buf + 1);
	strcat(p, "/");

	_counts[_side] = folder_list(_paths[_side], _xram_offs[_side]);
	manager_status_side(_side);
	manager_list_side(_side);
}

static void manager_tab(void)
{
	_side = !_side;
	manager_list_side(0);
	manager_list_side(1);
}
#endif

void manager_key(int key, int c)
{
	#if 0
	switch(key)
	{
	case KEY_HOME:  manager_home();  break;
	case KEY_END:   manager_end();   break;
	case KEY_UP:    manager_up();    break;
	case KEY_DOWN:  manager_down();  break;
	case KEY_ENTER: manager_enter(); break;
	case KEY_TAB:   manager_tab();   break;
	}
	#endif
}
