/* Symbol table implementation with a radix tree */
#include "symtabl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SYM_TAB
{
	struct SYM_TAB *Next;
	struct SYM_TAB *Children;
	char *Piece;
	symval Value;
} SymEntry;

/* --- PRIVATE --- */
static SymEntry *_new_entry(symval v)
{
	SymEntry *n = malloc(sizeof(SymEntry));
	n->Next = NULL;
	n->Children = NULL;
	n->Value = v;
	return n;
}

static char *_strdup(const char *s)
{
	size_t len = strlen(s);
	char *p = malloc(len + 1);
	strcpy(p, s);
	return p;
}

/* --- PUBLIC --- */
SymTab *symtab_create(void)
{
	SymEntry *tab = _new_entry(0);
	tab->Piece = _strdup("");
	tab->Value = 0;
	return tab;
}

void symtab_destroy(SymTab *tab)
{
	if(!tab)
	{
		return;
	}

	symtab_destroy(tab->Next);
	symtab_destroy(tab->Children);

	free(tab->Piece);
	free(tab);
}

int symtab_put(SymEntry *entry, const char *ident, symval value)
{
	char *edge;
	const char *search;

	/* Compare strings */
	search = ident;
	edge = entry->Piece;
	while(*edge && *search && *edge == *search)
	{
		++edge;
		++search;
	}

	if(!*edge)
	{
		/* End of edge */
		if(!*search)
		{
			/* Both are finished, symbol was found: Update value */
			entry->Value = (value << 1) | 1;
			return 1;
		}

		if(!entry->Children)
		{
			/* No children to search: Insert here */
			SymEntry *n = _new_entry(value);
			n->Piece = _strdup(search);
			n->Value = (value << 1) | 1;
			entry->Children = n;
			return 0;
		}

		/* Only edge piece is finished, follow this branch */
		return symtab_put(entry->Children, search, value);
	}
	else if(edge == entry->Piece)
	{
		/* Beginning of edge */
		if(!entry->Next)
		{
			/* No next entry: symbol not found */
			SymEntry *n = _new_entry(value);
			n->Piece = _strdup(search);
			n->Value = (value << 1) | 1;
			entry->Next = n;
			return 0;
		}

		/* No match, continue on same layer */
		return symtab_put(entry->Next, ident, value);
	}
	else
	{
		SymEntry *n, *second;

		/* Middle of edge: Split parent */
		second = _new_entry(entry->Value);
		second->Piece = _strdup(edge);
		second->Children = entry->Children;

		/* BUG ? overwrite value */

		entry->Value = 0;
		entry->Children = second;
		*edge = '\0';
		entry->Piece =
			realloc(entry->Piece, edge - entry->Piece + 1);

		n = _new_entry(value);
		n->Piece = _strdup(search);
		n->Value = (value << 1) | 1;

		second->Next = n;
		return 0;
	}
}

int symtab_remove(SymEntry *entry, const char *ident)
{
	SymTab *parent, *prev;
	const char *edge, *search;

	parent = NULL;
	prev = NULL;
	while(entry)
	{
		search = ident;
		edge = entry->Piece;
		while(*edge && *search && *edge == *search)
		{
			++edge;
			++search;
		}

		if(!*edge)
		{
			if(!*search && (entry->Value & 1))
			{
				SymEntry *child;

				/* Found: Delete */
				if(prev)
				{
					prev->Next = entry->Next;
				}
				else
				{
					parent->Children = entry->Next;
				}

				free(entry->Piece);
				free(entry);

				child = parent->Children;
				if(child && !(parent->Value & 1) && !child->Next)
				{
					/* Merge parent with child */

					size_t parent_len, child_len, len;

					parent->Value = (child->Value << 1) | 1;
					parent_len = strlen(parent->Piece);
					child_len = strlen(child->Piece);
					len = parent_len + child_len + 1;
					parent->Piece = realloc(parent->Piece, len);
					strcpy(parent->Piece + parent_len, child->Piece);

					free(child->Piece);
					free(child);
					parent->Children = NULL;
				}

				return 1;
			}

			prev = NULL;
			parent = entry;
			entry = entry->Children;
			ident = search;
		}
		else
		{
			prev = entry;
			entry = entry->Next;
		}
	}

	return 0;
}

int symtab_get(const SymEntry *entry, const char *ident, symval *value)
{
	const char *edge, *search;

	while(entry)
	{
		search = ident;
		edge = entry->Piece;
		while(*edge && *search && *edge == *search)
		{
			++edge;
			++search;
		}

		if(!*edge)
		{
			if(!*search && (entry->Value & 1))
			{
				*value = entry->Value >> 1;
				return 1;
			}

			entry = entry->Children;
			ident = search;
		}
		else
		{
			entry = entry->Next;
		}
	}

	return 0;
}

int symtab_exists(SymTab *tab, const char *ident)
{
	symval dummy;
	return symtab_get(tab, ident, &dummy);
}

int symtab_complete(SymTab *entry, char *ident)
{
	char *search;
	const char *edge;

	while(entry)
	{
		search = ident;
		edge = entry->Piece;
		while(*edge && *search && *edge == *search)
		{
			++edge;
			++search;
		}

		if(!*edge)
		{
			if(!*search)
			{
				return 0;
			}

			entry = entry->Children;
			ident = search;
		}
		else if(edge == entry->Piece)
		{
			entry = entry->Next;
		}
		else
		{
			while(*edge)
			{
				*search++ = *edge++;
			}

			*search = '\0';
			return 1;
		}
	}

	return 0;
}

int symtab_prefix_iter(SymTab *entry, char *ident, int max_results,
	void (*callback)(char *ident))
{
	int num_results = 0;

	/* TODO: Implement prefix iteration functionality */
	(void)entry;
	(void)ident;
	(void)max_results;
	(void)callback;
	return 0;
}

#ifdef SYMTAB_DEBUG

static void nspaces(int n)
{
	while(n--)
	{
		printf(" ");
	}
}

static void _symtab_print(SymEntry *entry, int nesting)
{
	while(entry)
	{
		nspaces(4 * nesting);
		printf("- %s", entry->Piece);
		if(entry->Value & 1)
		{
			printf(" = %d", entry->Value >> 1);
		}

		printf("\n");
		_symtab_print(entry->Children, nesting + 1);
		entry = entry->Next;
	}
}

void symtab_print(SymTab *tab)
{
	_symtab_print(tab->Children, 0);
}

#endif
