/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 * @brief   Template Project
 */

#include "init.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"
#define PEANUT_GB_HIGH_LCD_ACCURACY 0
#include "peanut_gb.h"


// Generate by xxd -i gameboy_rom.gb > gameboy_rom.h
// Adding static const in front so that the ROM data
// ends up read-only in flash memory.
static const
#include "gameboy_rom.h"

struct priv_t
{
	uint8_t *rom; /* Pointer to allocated memory holding GB file. */
	uint8_t *cart_ram; /* Pointer to allocated memory holding save file. */
};

uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr)
{
	const struct priv_t * const p = gb->direct.priv;
	return p->rom[addr];
}

uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr)
{
	const struct priv_t * const p = gb->direct.priv;
	return p->cart_ram[addr];
}

void gb_cart_ram_write(struct gb_s *gb, const uint_fast32_t addr,
			   const uint8_t val)
{
	const struct priv_t * const p = gb->direct.priv;
	p->cart_ram[addr] = val;
}

void gb_error(struct gb_s *gb, const enum gb_error_e gb_err, const uint16_t val)
{
	(void)gb, (void)gb_err, (void)val;
}

static const uint32_t palette[] =
{
	LCD_COLOR(155, 188, 15),
	LCD_COLOR(139, 172, 15),
	LCD_COLOR(48, 98, 48),
	LCD_COLOR(15, 56, 15)
};

static uint8_t buf[144][160];

void lcd_draw_line(struct gb_s *gb, const uint8_t pixels[160], const uint_fast8_t line)
{
	if(memcmp(buf[line], pixels, 160) == 0)
	{
		return;
	}

	lcd_window_start(0, line, GB_LCD_WIDTH, 1);
	for(int x = 0; x < GB_LCD_WIDTH; ++x)
	{
		lcd_emit(palette[pixels[x] & 3]);
	}

	lcd_window_end();
	memcpy(buf[line], pixels, 160);
	(void)gb;
}

#define SETBIT(w, m, f) if (f) { w |= m; } else { w &= ~m; }
static void checkbuttons(struct gb_s* gb)
{
	int b = GPIOF->IDR;

	SETBIT(gb->direct.joypad, JOYPAD_START,  (b & 0x80));
	SETBIT(gb->direct.joypad, JOYPAD_SELECT, (b & 0x40));
	SETBIT(gb->direct.joypad, JOYPAD_A,      (b & 0x20));
	SETBIT(gb->direct.joypad, JOYPAD_B,      (b & 0x10));
	SETBIT(gb->direct.joypad, JOYPAD_LEFT,   (b & 0x08));
	SETBIT(gb->direct.joypad, JOYPAD_UP,     (b & 0x04));
	SETBIT(gb->direct.joypad, JOYPAD_DOWN,   (b & 0x02));
	SETBIT(gb->direct.joypad, JOYPAD_RIGHT,  (b & 0x01));
}

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	its_board_init();
	timer_init();
	lcd_init(COLOR_BLACK);

	static struct gb_s gb;
	static struct priv_t priv;
	static uint8_t g_ram[0x20000];

	priv.rom = (uint8_t*)gameboy_rom_gb;
	priv.cart_ram = g_ram;

	gb_init(&gb, &gb_rom_read, &gb_cart_ram_read,
			&gb_cart_ram_write, &gb_error, &priv);

	gb_init_lcd(&gb, &lcd_draw_line);
	for(;;)
	{
		checkbuttons(&gb);
		gb_run_frame(&gb);
	}

	return 0;
}
