#include "game.h"
#include "player.h"
#include "level.h"
#include "keys.h"
#include "delay.h"

static int level_id;

static void game_tick(Player *player)
{
	int keys = keys_get();
	player_move(player, keys);

	int ret = level_collision(player);
	if(ret)
	{
		if(ret == 2)
		{
			++level_id;
			if(level_id >= LEVEL_COUNT)
			{
				level_id = 0;
			}

			level_load(level_id);
			level_draw();
		}

		level_spawn_player(player);
	}

	player_draw(player);
	delay_ms(10);
}

void game_loop(void)
{
	Player player;

	level_load(0);
	level_draw();
	level_spawn_player(&player);

	for(;;)
	{
		game_tick(&player);
	}
}
