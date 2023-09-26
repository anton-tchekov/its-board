/**
 * @file    sd_ll.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-25
 * @brief   SD card low-level interface
 */

#ifndef __SD_LL_H__
#define __SD_LL_H__

#define LCD_SD_CS       ?

static inline void lcd_sd_cs_0(void)
{
	GPIOD->BSRR |= (1 << (LCD_SD_CS + 16));
}

static inline void lcd_sd_cs_1(void)
{
	GPIOD->BSRR |= (1 << LCD_SD_CS);
}

#endif /* __SD_LL_H__ */
