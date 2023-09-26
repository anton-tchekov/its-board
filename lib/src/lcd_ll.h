/**
 * @file    lcd_ll.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-25
 * @brief   LCD low-level interface
 */

#ifndef __LCD_LL_H__
#define __LCD_LL_H__

#include "stm32f4xx_hal.h"

#define LCD_RST_PORT      GPIOF->BSRR
#define LCD_RST         12

#define LCD_DC_PORT       GPIOF->BSRR
#define LCD_DC          13

#define LCD_CS_PORT       GPIOD->BSRR
#define LCD_CS          14

static inline void lcd_rst_0(void)
{
	LCD_RST_PORT |= (1 << (LCD_RST + 16));
}

static inline void lcd_rst_1(void)
{
	LCD_RST_PORT |= (1 << LCD_RST);
}

static inline void lcd_dc_0(void)
{
	LCD_DC_PORT |= (1 << (LCD_DC + 16));
}

static inline void lcd_dc_1(void)
{
	LCD_DC_PORT |= (1 << LCD_DC);
}

static inline void lcd_cs_0(void)
{
	LCD_CS_PORT |= (1 << (LCD_CS + 16));
}

static inline void lcd_cs_1(void)
{
	LCD_CS_PORT |= (1 << LCD_CS);
}

#endif /* __LCD_LL_H__ */
