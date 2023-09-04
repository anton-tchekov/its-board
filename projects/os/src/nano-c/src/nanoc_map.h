#ifndef __NANOC_MAP_H__
#define __NANOC_MAP_H__

#include "types.h"
#include "nanoc_status.h"

typedef struct NANOC_MAP_ELEMENT
{
	const char *Key;
	size_t Length;
} NanoC_MapElement;

typedef struct NANOC_MAP
{
	NanoC_MapElement *Elements;
	size_t Count, Capacity;
} NanoC_Map;

void nanoc_map_init(NanoC_Map *map, NanoC_MapElement *buf, size_t capacity);
u8r nanoc_map_find(NanoC_Map *map, const char *key, size_t len, size_t *idx);
u8r nanoc_map_insert(NanoC_Map *map, const char *key, size_t len, size_t *idx);

#endif /* __NANOC_MAP_H__ */
