#include "collision.h"
#include "tile.h"
#include "util.h"
#include "player.h"

int collides(Player *player, int x, int y, int w, int h)
{
	int r1x1 = scale(player->PosX);
	int r1x2 = r1x1 + TILE_SIZE;
	int r1y1 = scale(player->PosY);
	int r1y2 = r1y1 + TILE_SIZE;
	int r2x2 = x + w;
	int r2y2 = y + h;

	return ((r1x1 <= r2x2) &&
		(x <= r1x2) &&
		(r1y1 <= r2y2) &&
		(y <= r1y2));
}

void collision_solid(Player *player, int x, int y, int w, int h)
{
	int x1 = player->PosX;
	int y1 = player->PosY;
	int w1 = unscale(TILE_SIZE);
	int h1 = unscale(TILE_SIZE);

	int x2 = unscale(x);
	int y2 = unscale(y);
	int w2 = unscale(w);
	int h2 = unscale(h);

	int vx = (x1 + (w1 / 2)) - (x2 + (w2 / 2));
	int vy = (y1 + (h1 / 2)) - (y2 + (h2 / 2));
	int hw = (w1 / 2) + (w2 / 2);
	int hh = (h1 / 2) + (h2 / 2);

	if(abs(vx) < hw && abs(vy) < hh)
	{
		int ox = hw - abs(vx);
		int oy = hh - abs(vy);
		if(ox >= oy)
		{
			if(vy > 0)
			{
				player->PosY += oy;
				player->VelocityY *= -1;
			}
			else
			{
				player->PosY -= oy;
				player->Jumping = 0;
				player->Grounded = 1;
				player->VelocityY = 0;
			}
		}
		else
		{
			if(vx > 0)
			{
				player->PosX += ox;
			}
			else
			{
				player->PosX -= ox;
			}

			player->Jumping = 0;
			player->Grounded = 1;
			player->VelocityX = 0;
		}
	}
}
