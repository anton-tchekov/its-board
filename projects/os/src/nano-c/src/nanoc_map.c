/**
 * @file    nanoc_map.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_map.h"
#include "nanoc_status.h"
#include <string.h>

static void map_elem_set(NanoC_MapElement *elem, const char *key, size_t len)
{
	elem->Key = key;
	elem->Length = len;
}

static NanoC_Bool map_elem_equals(
	NanoC_MapElement *elem, const char *key, size_t len)
{
	return (len == elem->Length) && !strncmp(elem->Key, key, len);
}

NanoC_Bool nanoc_map_find(
	NanoC_Map *map, const char *key, size_t len, size_t *idx)
{
	size_t i;
	for(i = 0; i < map->Count; ++i)
	{
		if(map_elem_equals(&map->Elements[i], key, len))
		{
			*idx = i;
			return 1;
		}
	}

	return 0;
}

NanoC_Status nanoc_map_insert(NanoC_Map *map, const char *key, size_t len,
	size_t *idx)
{
	if(map->Count >= map->Capacity)
	{
		return NANOC_ERROR_OVERFLOW;
	}

	if(nanoc_map_find(map, key, len, idx))
	{
		return NANOC_ERROR_REDEFINITION;
	}

	map_elem_set(map->Elements + map->Count, key, len);
	*idx = map->Count++;
	return NANOC_STATUS_SUCCESS;
}

void nanoc_map_init(NanoC_Map *map, NanoC_MapElement *buf, size_t capacity)
{
	map->Elements = buf;
	map->Count = 0;
	map->Capacity = capacity;
}
