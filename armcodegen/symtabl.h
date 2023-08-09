#ifndef __SYMTABL_H__
#define __SYMTABL_H__

#include <stdint.h>
#include <stddef.h>

/* Symbol table interface */
typedef uint32_t symval;
typedef struct SYM_ENTRY
{
	struct SYM_ENTRY *Next;
	struct SYM_ENTRY *Children;
	const char *Piece;
	symval Value;
	int IsLeaf;
} SymEntry;

void symtab_init(SymEntry *tab);

/**
 * @brief Inserts/updates the value for a symbol
 *
 * @param tab Symbol table
 * @param ident Symbol identifier
 * @param value Value
 * @return true if the symbol already existed
 */
int symbol_put(SymEntry *tab, const char *ident, symval value);

/**
 * @brief Removes a symbol
 *
 * @param tab Symbol table
 * @param ident Symbol identifier
 * @return true if the symbol existed
 */
int symbol_remove(SymEntry *tab, const char *ident);

/**
 * @brief Gets the value for a symbol
 *
 * @param tab Symbol table
 * @param ident Symbol identifier
 * @param value Pointer for the result
 * @return true if the symbol existed, false if the symbol was not found
 */
int symbol_get(SymEntry *root, const char *ident, symval *value);


void symtab_print(SymEntry *root);

/**
 * @brief Gets the value for a symbol
 *
 * @param tab Symbol table
 * @param ident Symbol identifier
 * @param value Pointer for the result
 * @return true if the symbol existed, false if the symbol was not found
 */
//int symbol_search(SymTab *tab, const char *prefix, const char *cmpl,
//	int max_results);

#endif /* __SYMTABL_H__ */
