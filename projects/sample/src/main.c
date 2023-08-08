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
#include "font.h"
#include "fonts/Terminus16.h"
#include "fonts/Terminus16_Bold.h"
#include "ps2.h"

volatile int flag = 0;

void _phase_change(void)
{
	flag = 1;
}


/**
 * @brief ISR fuer GPIOF-Pin 0 (steigende und fallende Flanke)
 */
void EXTI0_IRQHandler(void)
{
	EXTI->PR |= (1 << 0);
	_phase_change();
}

/**
 * @brief ISR fuer GPIOF-Pin 1 (steigende und fallende Flanke)
 */
void EXTI1_IRQHandler(void)
{
	EXTI->PR |= (1 << 1);
	_phase_change();
}

static void _interrupt_init(void)
{
	/* Clock for GPIO Port G */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	/* System conf. clock enable */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/* Routing Pin 0,1 of Port G -> EXTI0,1 */
	SYSCFG->EXTICR[0] &= ~((0x0f << (4 * 0)) | (0x0f << (4 * 1)));
	SYSCFG->EXTICR[0] |= ((0x05 << (4 * 0)) | (0x05 << (4 * 1)));

	/* Enable rising and falling trigger */
	EXTI->RTSR |= (1 << 0) | (1 << 1);
	EXTI->FTSR |= (1 << 0) | (1 << 1);
	EXTI->IMR |= (1 << 0) | (1 << 1);

	NVIC_SetPriorityGrouping(2);
	NVIC_SetPriority(EXTI0_IRQn, 8);
	NVIC_EnableIRQ(EXTI0_IRQn);

	NVIC_SetPriority(EXTI1_IRQn, 8);
	NVIC_EnableIRQ(EXTI1_IRQn);
}




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

#define LOGO_WIDTH   160
#define LOGO_HEIGHT  200

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	_interrupt_init();
	timer_init();
	lcd_init(D2U_L2R, 1000, COLOR_BLACK);
	lcd_rect(20, 20, 80, 20, COLOR_RED);
#if 0
	int cx = 30;
	//ps2_init();
	//__enable_irq();


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

	KeyEvent event;
	for(;;)
	{
		if(ps2_read(&event))
		{
			font_char(cx, 50, event.Codepoint,
				COLOR_WHITE, COLOR_BLACK, Terminus16);

			cx += 8;
		}

	}
#endif

	draw_fancy_logo(LCD_WIDTH / 2 - LOGO_WIDTH / 2,
		LCD_HEIGHT / 2 - LOGO_HEIGHT / 2,
		LOGO_WIDTH, LOGO_HEIGHT, 0);

	int pos = 10;
	for(;;)
	{
		if(flag)
		{
			font_str(100, pos, "Hello World\xFF\xFF\xFF", COLOR_WHITE, COLOR_BLACK, Terminus16);
			flag = 0;
			pos += 20;
		}
	}

	return 0;
}
