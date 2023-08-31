#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "player.h"

int collides(Player *player, int x, int y, int w, int h);
void collision_solid(Player *player, int x, int y, int w, int h);

#endif /* __COLLISION_H__ */
