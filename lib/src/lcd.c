/**
 * @file    lcd.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-21
 */

#include "lcd.h"
#include "delay.h"
#include "stm32f4xx_hal.h"

#define LCD_RST_0         GPIOF->BSRR |= (1 << (12 + 16))
#define LCD_RST_1         GPIOF->BSRR |= (1 << 12)

#define LCD_DC_0          GPIOF->BSRR |= (1 << (13 + 16))
#define LCD_DC_1          GPIOF->BSRR |= (1 <<  13)

#define LCD_CS_0          GPIOD->BSRR |= (1 << (14 + 16))
#define LCD_CS_1          GPIOD->BSRR |= (1 << 14)

#define GPIO_MODE_AF      0x02
#define GPIO_SPEED_50MHZ  0x02
#define GPIO_PUPD_DOWN    0x02

static const uint8_t _lcd_init_cmds[] =
{
	0xF9,
	2,
	0x00,
	0x08,
	0xC0,
	2,
	0x19,
	0x1a,
	0xC1,
	2,
	0x45,
	0x00,
	0xC2,
	1,
	0x33,
	0xC5,
	2,
	0x00,
	0x28,
	0xB1,
	2,
	0xA0,
	0x11,
	0xB4,
	1,
	0x02,
	0xB6,
	3,
	0x00,
	0x42,
	0x3B,
	0xB7,
	1,
	0x07,
	0xE0,
	15,
	0x1F,
	0x25,
	0x22,
	0x0B,
	0x06,
	0x0A,
	0x4E,
	0xC6,
	0x39,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0xE1,
	15,
	0x1F,
	0x3F,
	0x3F,
	0x0F,
	0x1F,
	0x0F,
	0x46,
	0x49,
	0x31,
	0x05,
	0x09,
	0x03,
	0x1C,
	0x1A,
	0x00,
	0xF1,
	8,
	0x36,
	0x04,
	0x00,
	0x3C,
	0x0F,
	0x0F,
	0xA4,
	0x02,
	0xF2,
	9,
	0x18,
	0xA3,
	0x12,
	0x02,
	0x32,
	0x12,
	0xFF,
	0x32,
	0x00,
	0xF4,
	5,
	0x40,
	0x00,
	0x08,
	0x91,
	0x04,
	0xF8,
	2,
	0x21,
	0x04,
	0x3A,
	1,
	0x55
};

int lcd_byte(int byte)
{
	while(!(SPI1->SR & SPI_SR_TXE)) {}
	SPI1->DR = byte;
	while(!(SPI1->SR & SPI_SR_RXNE)) {}
	while(SPI1->SR & SPI_SR_BSY) {}
	return SPI1->DR;
}

static void lcd_param0(void)
{
	LCD_DC_1;
	LCD_CS_0;
	lcd_byte(0);
}

static void lcd_param1(int param)
{
	lcd_byte(param);
	LCD_CS_1;
}

void lcd_param(int param)
{
	lcd_param0();
	lcd_param1(param);
}

void lcd_cmd(int cmd)
{
	LCD_DC_0;
	LCD_CS_0;
	lcd_byte(cmd);
	LCD_CS_1;
}

void lcd_emit(int color)
{
	lcd_byte(color >> 8);
	lcd_byte(color & 0xFF);
}

void lcd_reset(void)
{
	LCD_RST_1;
	delay_ms(500);
	LCD_RST_0;
	delay_ms(500);
	LCD_RST_1;
	delay_ms(500);
}

static void lcd_cmd_list(const uint8_t *list, int count)
{
	int num;
	int elem = 0;
	while(elem < count)
	{
		lcd_cmd(list[elem++]);
		num = list[elem++];
		while(num--)
		{
			lcd_param(list[elem++]);
		}
	}
}

void lcd_set_orientation(int orientation)
{
	int a, b;
	switch(orientation)
	{
	case L2R_U2D:
		a = 0x08;
		b = 0x22;
		break;

	case L2R_D2U:
		a = 0x08;
		b = 0x62;
		break;

	case R2L_U2D:
		a = 0x08;
		b = 0x02;
		break;

	case R2L_D2U:
		a = 0x08;
		b = 0x42;
		break;

	case U2D_L2R:
		a = 0x28;
		b = 0x22;
		break;

	default:
	case U2D_R2L:
		a = 0x28;
		b = 0x02;
		break;

	case D2U_L2R:
		a = 0x28;
		b = 0x62;
		break;

	case D2U_R2L:
		a = 0x28;
		b = 0x42;
		break;
	}

	lcd_cmd(0xB6);
	lcd_param(0x00);
	lcd_param(b);
	lcd_cmd(0x36);
	lcd_param(a);
}

void lcd_set_backlight(int value)
{
}

void lcd_window_start(int x, int y, int w, int h)
{
	int ex = x + w - 1;
	int ey = y + h - 1;

	lcd_cmd(0x2A);
	lcd_param(x >> 8);
	lcd_param(x & 0xFF);
	lcd_param(ex >> 8);
	lcd_param(ex & 0xFF);

	lcd_cmd(0x2B);
	lcd_param(y >> 8);
	lcd_param(y & 0xFF);
	lcd_param(ey >> 8);
	lcd_param(ey & 0xFF);

	lcd_cmd(0x2C);
	LCD_DC_1;
	LCD_CS_0;
}

void lcd_window_end(void)
{
	LCD_CS_1;
}

void lcd_rect(int x, int y, int w, int h, int color)
{
	int count = w * h;
	int color_hi = color >> 8;
	int color_lo = color & 0xFF;

	lcd_window_start(x, y, w, h);
	while(count--)
	{
		lcd_byte(color_hi);
		lcd_byte(color_lo);
	}

	lcd_window_end();
}

void lcd_callback(int x, int y, int w, int h, void *handle,
	int (*callback)(int, int, void *))
{
	int x0, y0;

	lcd_window_start(x, y, w, h);
	for(y0 = 0; y0 < h; ++y0)
	{
		for(x0 = 0; x0 < w; ++x0)
		{
			lcd_emit(callback(x0, y0, handle));
		}
	}

	lcd_window_end();
}

static void lcd_gpio_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER |=
		(GPIO_MODE_AF << (5 * 2)) |
		(GPIO_MODE_AF << (6 * 2)) |
		(GPIO_MODE_AF << (7 * 2));

	GPIOA->OSPEEDR |=
		(GPIO_SPEED_50MHZ << (5 * 2)) |
		(GPIO_SPEED_50MHZ << (6 * 2)) |
		(GPIO_SPEED_50MHZ << (7 * 2));

	GPIOA->PUPDR |= (GPIO_PUPD_DOWN << (5 * 2));

	GPIOA->AFR[0] |=
		(GPIO_AF5_SPI1 << (5 * 4)) |
		(GPIO_AF5_SPI1 << (6 * 4)) |
		(GPIO_AF5_SPI1 << (7 * 4));
}

static void lcd_spi_init(uint32_t baudrate)
{
	baudrate &= SPI_CR1_BR_Msk;
	SPI1->CR1 &= ~(1 <<  6);
	SPI1->CR1 = (1 << 9) | (1 << 8) | baudrate | (1 << 2);
	SPI1->CR2 = 0;
	SPI1->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;
	SPI1->CR1 |= (1 << 6);
}

void lcd_init(int orientation, int backlight, int color)
{
	lcd_gpio_init();
	lcd_spi_init(SPI_BAUDRATEPRESCALER_16);
	lcd_reset();
	lcd_set_backlight(backlight);
	lcd_cmd_list(_lcd_init_cmds, sizeof(_lcd_init_cmds));
	lcd_set_orientation(orientation);
	delay_ms(200);
	lcd_cmd(0x11);
	delay_ms(120);
	lcd_cmd(0x29);
	lcd_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

int lcd_color(int r, int g, int b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
