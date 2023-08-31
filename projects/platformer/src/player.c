#include "player.h"
#include "keys.h"
#include "tile.h"
#include "util.h"
#include "level.h"

#define PLAYER_MAX_SPEED           FP( 7,50)
#define PLAYER_ACCELERATION        FP( 2,20)
#define PLAYER_JUMP_ACCELERATION   FP(-5,50)
#define PLAYER_FRICTION            FP( 0,95)
#define PLAYER_GRAVITY             FP( 0,15)

void player_spawn(Player *player, int x, int y)
{
	player->Facing = FACING_RIGHT;
	player->Jumping = 0;
	player->Grounded = 0;

	player->PosX = unscale(x);
	player->PosY = unscale(y);

	player->VelocityX = 0;
	player->VelocityY = 0;

	player->Facing = FACING_RIGHT;

	tile_draw(TILE_PLAYER_R, x, y);
}

void player_draw(Player *player)
{
	int player_tile = (player->Facing == FACING_LEFT) ?
		TILE_PLAYER_L : TILE_PLAYER_R;

	int cur_x = scale(player->PosX);
	int cur_y = scale(player->PosY);

	if((cur_x != player->PrevX) || (cur_y != player->PrevY))
	{
		level_restore(player->PrevX, player->PrevY, TILE_SIZE, TILE_SIZE);
		tile_draw(player_tile, cur_x, cur_y);
	}
}

static void player_key_up(Player *player)
{
	if(!player->Jumping && player->Grounded)
	{
		player->Jumping = 1;
		player->Grounded = 0;
		player->VelocityY = PLAYER_JUMP_ACCELERATION;
	}
}

static void player_key_left(Player *player)
{
	player->Facing = FACING_LEFT;
	player->VelocityX = max(-PLAYER_MAX_SPEED,
		player->VelocityX - PLAYER_ACCELERATION);
}

static void player_key_right(Player *player)
{
	player->Facing = FACING_RIGHT;
	player->VelocityX = min(PLAYER_MAX_SPEED,
		player->VelocityX + PLAYER_ACCELERATION);
}

static void player_update(Player *player)
{
	player->PosX += player->VelocityX;
	player->PosY += player->VelocityY;

	player->VelocityY += PLAYER_GRAVITY;
	player->VelocityX = scale(player->VelocityX * PLAYER_FRICTION);
}

void player_move(Player *player, int keys)
{
	player->PrevX = scale(player->PosX);
	player->PrevY = scale(player->PosY);

	if(keys & KEY_MASK_UP)
	{
		player_key_up(player);
	}

	if(keys & KEY_MASK_LEFT)
	{
		player_key_left(player);
	}

	if(keys & KEY_MASK_RIGHT)
	{
		player_key_right(player);
	}

	player_update(player);
}
