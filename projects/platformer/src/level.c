#include "level.h"
#include "tile.h"
#include "util.h"
#include "collision.h"
#include "lcd.h"
#include <stdint.h>

extern const char levels[];

static int _start_x, _start_y, _exit_x, _exit_y;
static uint8_t level[LEVEL_WIDTH * LEVEL_HEIGHT];

int tile_type(int chr)
{
	static const char tile_types[] = " #AVLHabcd1234";
	const char *p = tile_types;
	int c;
	while((c = *p) && c != chr) { ++p; }
	return p - tile_types;
}

static inline uint8_t *level_ptr(int x, int y)
{
	return &level[y * LEVEL_WIDTH + x];
}

typedef struct RESTORE_DATA
{
	int OffsetX;
	int OffsetY;
} RestoreData;

static int restore_callback(int x, int y, void *data)
{
	RestoreData *pos = data;

	int gx = pos->OffsetX + x;
	int gy = pos->OffsetY + y;

	int tile_x = gx / TILE_SIZE;
	int tile_y = gy / TILE_SIZE;

	int tile = *level_ptr(tile_x, tile_y);

	int in_x = gx - tile_x * TILE_SIZE;
	int in_y = gy - tile_y * TILE_SIZE;

	TileData td;
	tile_data(tile, &td);
	return tile_color_at(in_x, in_y, &td);
}

void level_restore(int x, int y, int w, int h)
{
	RestoreData data =
	{
		.OffsetX = x,
		.OffsetY = y
	};

	lcd_callback(x, y, w, h, &data, restore_callback);
}

void level_load(int level_id)
{
	const char *offset = levels + (level_id * LEVEL_WIDTH * LEVEL_HEIGHT);
	uint8_t *dest = level;
	int x, y;
	for(y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for(x = 0; x < LEVEL_WIDTH; ++x)
		{
			int tt = tile_type(*offset);
			if(tt == TILE_TYPE_START_TL)
			{
				_start_x = x * TILE_SIZE;
				_start_y = y * TILE_SIZE;
			}
			else if(tt == TILE_TYPE_EXIT_TL)
			{
				_exit_x = x * TILE_SIZE;
				_exit_y = y * TILE_SIZE;
			}

			*dest = tt;
			++dest;
			++offset;
		}
	}
}

void level_draw(void)
{
	const uint8_t *offset = level;
	int x, y;
	for(y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for(x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile_draw(*offset, x * TILE_SIZE, y * TILE_SIZE);
			++offset;
		}
	}
}

static int exit_collision(Player *player)
{
	return collides(player, _exit_x, _exit_y, 2 * TILE_SIZE, 2 * TILE_SIZE);
}

static int tile_collision(Player *player, int x, int y)
{
	uint8_t *lp = level_ptr(x, y);
	int tile = *lp;
	int xp = x * TILE_SIZE;
	int yp = y * TILE_SIZE;
	int fail = 0;

	switch(tile_get_type(tile))
	{
	case TILE_TYPE_INVISIBLE:
		if(collides(player, xp, yp, TILE_SIZE, TILE_SIZE))
		{
			*lp = tile_set_opacity(tile, OPAQUE);
		}
		/* fall through */

	case TILE_TYPE_SOLID:
		collision_solid(player, xp, yp, TILE_SIZE, TILE_SIZE);
		break;

	case TILE_TYPE_SPIKE_UP:
	case TILE_TYPE_SPIKE_DOWN:
	case TILE_TYPE_LAVA:
		if(collides(player, xp + 2, yp + 2, TILE_SIZE - 4, TILE_SIZE - 4))
		{
			fail = 1;
		}
		break;
	}

	return fail;
}

void level_spawn_player(Player *player)
{
	player_spawn(player, _start_x, _start_y);
}

int level_collision(Player *player)
{
	int x, y;

	int ptx = scale(player->PosX) / TILE_SIZE;
	int start_x = min(0, ptx - 2);
	int end_x = max(LEVEL_WIDTH, ptx + 3);

	int pty = scale(player->PosY) / TILE_SIZE;
	int start_y = min(0, pty - 2);
	int end_y = max(LEVEL_HEIGHT, pty + 3);

	if(exit_collision(player))
	{
		return 2;
	}

	for(y = start_y; y < end_y; ++y)
	{
		for(x = start_x; x < end_x; ++x)
		{
			if(tile_collision(player, x, y))
			{
				return 1;
			}
		}
	}

	return 0;
}
