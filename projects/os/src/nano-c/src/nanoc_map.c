#include "nanoc_map.h"
#include "nanoc_instruction.h"
#include "nanoc_status.h"
#include <string.h>

static void map_elem_set(NanoC_MapElement *elem, const char *key, size_t len)
{
	elem->Key = key;
	elem->Length = len;
}

static u8r map_elem_equals(NanoC_MapElement *elem, const char *key, size_t len)
{
	return (len == elem->Length) && !strncmp(elem->Key, key, len);
}

u8r nanoc_map_find(NanoC_Map *map, const char *key, size_t len, size_t *idx)
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

u8r nanoc_map_insert(NanoC_Map *map, const char *key, size_t len, size_t *idx)
{
	if(map->Count >= map->Capacity)
	{
		return NANOC_ERROR_STACK_OVERFLOW;
	}

	if(nanoc_map_find(map, key, len, idx))
	{
		return NANOC_ERROR_DUPLICATE_MAP_ELEMENT;
	}

	map_elem_set(&map->Elements[map->Count], key, len);
	*idx = map->Count++;
	return NANOC_STATUS_SUCCESS;
}

void nanoc_map_init(NanoC_Map *map, NanoC_MapElement *buf, size_t capacity)
{
	map->Elements = buf;
	map->Count = 0;
	map->Capacity = capacity;
}
