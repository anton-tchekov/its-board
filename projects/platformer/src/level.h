#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "player.h"

#define LEVEL_COUNT   13
#define LEVEL_WIDTH   40
#define LEVEL_HEIGHT  20

void level_load(int level_id);
void level_draw(void);
int level_collision(Player *player);
void level_spawn_player(Player *player);
void level_restore(int x, int y, int w, int h);

#endif /* __LEVEL_H__ */
