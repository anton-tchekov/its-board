#include "tile.h"
#include "lcd.h"

extern const char tiles[];
extern const uint16_t colors[];

void tile_data(int tile, TileData *data)
{
	int tile_type = tile_get_type(tile);
	int tile_opacity = tile_get_opacity(tile);
	data->Sprite = tiles + (tile_type * TILE_SIZE * TILE_SIZE);
	data->Opacity = tile_opacity;
}

int tile_color_at(int x, int y, TileData *data)
{
	int color_index = data->Sprite[y * TILE_SIZE + x] - 'A';
	return colors[color_index];
}

static int tile_callback(int x, int y, void *data)
{
	return tile_color_at(x, y, data);
}

void tile_draw(int tile, int x, int y)
{
	TileData data;
	tile_data(tile, &data);
	lcd_callback(x, y, TILE_SIZE, TILE_SIZE, &data, tile_callback);
}
