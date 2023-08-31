/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-08-28
 * @brief   Platformer game
 */

#include "init.h"
#include "timer.h"
#include "delay.h"
#include "lcd.h"

#include "keys.h"
#include "tile.h"
#include "level.h"
#include "player.h"
#include "game.h"

/**
 * @brief Main function
 * @return Unreachable
 */
int main(void)
{
	its_board_init();
	timer_init();
	lcd_init(D2U_L2R, 1000, COLOR_BLACK);
	game_loop();

	return 0;
}
