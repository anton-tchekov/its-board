#ifndef __TILE_H__
#define __TILE_H__

#define TILE_SIZE   12
#define OPAQUE      15
#define TRANSPARENT  0

typedef enum
{
	TILE_TYPE_AIR,
	TILE_TYPE_SOLID,
	TILE_TYPE_SPIKE_UP,
	TILE_TYPE_SPIKE_DOWN,
	TILE_TYPE_LAVA,
	TILE_TYPE_INVISIBLE,

	TILE_TYPE_START_TL,
	TILE_TYPE_START_TR,
	TILE_TYPE_START_BL,
	TILE_TYPE_START_BR,

	TILE_TYPE_EXIT_TL,
	TILE_TYPE_EXIT_TR,
	TILE_TYPE_EXIT_BL,
	TILE_TYPE_EXIT_BR,

	TILE_PLAYER_R,
	TILE_PLAYER_L,
} TileType;

typedef struct
{
	int Opacity;
	const char *Sprite;
} TileData;

static inline int tile_get_type(int tile)
{
	return tile & 0x0F;
}

static inline int tile_get_opacity(int tile)
{
	return tile >> 4;
}

static inline int tile_set_opacity(int tile, int opacity)
{
	tile &= 0x0F;
	tile |= (opacity << 4);
	return tile;
}

void tile_draw(int tile, int x, int y);
void tile_data(int tile, TileData *data);
int tile_color_at(int x, int y, TileData *data);

#endif /* __TILE_H__ */
