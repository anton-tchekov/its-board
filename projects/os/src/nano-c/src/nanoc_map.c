#include "nanoc_map.h"
#include "nanoc_status.h"
#include "ctype_ext.h"
#include <string.h>

static NanoC_Bool nanoc_map_compare(const char *p, const char *q)
{
	while(is_ident(*p) || is_ident(*q))
	{
		if(*p != *q)
		{
			return 0;
		}

		++p;
		++q;
	}

	return 1;
}

ssize_t nanoc_map_find(NanoC_Map *map, const char *key)
{
	size_t i;
	for(i = 0; i < map->Count; ++i)
	{
		if(nanoc_map_compare(key, map->Elements[i].Key))
		{
			return i;
		}
	}

	return NANOC_ERROR_UNDEFINED;
}

ssize_t nanoc_map_insert(NanoC_Map *map, const char *key)
{
	if(map->Count >= map->Capacity)
	{
		NANOC_THROW(NANOC_ERROR_OVERFLOW);
	}

	if(nanoc_map_find(map, key) >= 0)
	{
		NANOC_THROW(NANOC_ERROR_REDEFINITION);
	}

	map->Elements[map->Count].Key = key;
	return map->Count++;
}

ssize_t nanoc_map_insget(NanoC_Map *map, const char *key)
{
	ssize_t idx;
	if(map->Count >= map->Capacity)
	{
		NANOC_THROW(NANOC_ERROR_OVERFLOW);
	}

	idx = nanoc_map_find(map, key);
	if(idx >= 0)
	{
		return idx;
	}

	map->Elements[map->Count].Key = key;
	return map->Count++;
}

void nanoc_map_init(NanoC_Map *map, NanoC_MapElement *buf, size_t capacity)
{
	map->Elements = buf;
	map->Count = 0;
	map->Capacity = capacity;
}
