/* Symbol table implementation with a radix tree */
#include "symtabl.h"
#include <stdio.h>

/*

horse
house
hound

ho - rse
   - u - nd
       - se

*/



void symtab_init(SymEntry *tab)
{
	static SymEntry
	SE = {
		NULL,
		NULL,
		"se",
		777,
		1
	},
	ND = {
		&SE,
		NULL,
		"nd",
		23,
		1
	},
	U = {
		NULL,
		&ND,
		"u",
		0,
		0
	},
	RSE = {
		&U,
		NULL,
		"rse",
		42,
		1
	};

	tab->Next = NULL;
	tab->Children = &RSE;
	tab->Piece = "ho";
	tab->Value = 0;
	tab->IsLeaf = 0;
}

int symbol_put(SymEntry *tab, const char *ident, symval value)
{
	return 0;
}

int symbol_remove(SymEntry *tab, const char *ident)
{
	return 0;
}

int symbol_get(SymEntry *entry, const char *ident, symval *value)
{
	const char *edge, *search;

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
		if(!*search && entry->IsLeaf)
		{
			/* Both are finished, symbol was found */
			*value = entry->Value;
			return 1;
		}

		if(!entry->Children)
		{
			/* No children to search */
			return 0;
		}

		/* Only edge piece is finished, follow this branch */
		return symbol_get(entry->Children, search, value);
	}
	else
	{
		if(!entry->Next)
		{
			/* No next entry: symbol not found */
			return 0;
		}

		/* No match, continue on same layer */
		return symbol_get(entry->Next, ident, value);
	}
}

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
		printf("- %s\n", entry->Piece);
		_symtab_print(entry->Children, nesting + 1);
		entry = entry->Next;
	}
}

void symtab_print(SymEntry *root)
{
	_symtab_print(root, 0);
}
