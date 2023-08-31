#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef struct PLAYER
{
	int PrevX;
	int PrevY;
	int PosX;
	int PosY;
	int VelocityX;
	int VelocityY;
	int Facing;
	int Jumping;
	int Grounded;
} Player;

typedef enum FACING
{
	FACING_LEFT,
	FACING_RIGHT
} Facing;

void player_init(Player *player);
void player_spawn(Player *player, int x, int y);
void player_move(Player *player, int keys);
void player_draw(Player *player);

#endif /* __PLAYER_H__ */
