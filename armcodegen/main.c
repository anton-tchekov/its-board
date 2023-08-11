#include <stdio.h>
#include <ctype.h>
#include "symtabl.h"

void iter_callback(char *s)
{
	printf("Suggestion: %s\n", s);
}

void test_iteration(SymTab *tab)
{
	char buf[256] = "lcd_";
	int rv = symtab_prefix_iter(tab, buf, 0, iter_callback);
	printf("Results: %d\n", rv);
}

int main(void)
{
	char test[] = "Radix trees support insertion, deletion, and searching operations. Insertion adds a new string to the trie while trying to minimize the amount of data stored. Deletion removes a string from the trie. Searching operations include (but are not necessarily limited to) exact lookup, find predecessor, find successor, and find all strings with a prefix. All of these operations are O(k) where k is the maximum length of all strings in the set, where length is measured in the quantity of bits equal to the radix of the radix trie. ";

	SymTab *tab;
	tab = symtab_create();

	symtab_put(tab, "lcd_init", 1);
	symtab_put(tab, "lcd_set_orientation", 1);
	symtab_put(tab, "lcd_rect", 1);
	symtab_put(tab, "lcd_callback", 1);
	symtab_put(tab, "lcd_clear", 1);
	symtab_put(tab, "lcd_set_backlight", 1);

	symtab_print(tab);
	symtab_remove(tab, "lcd_set_backlight");
	symtab_print(tab);
	symtab_remove(tab, "lcd_clear");
	symtab_print(tab);
	symtab_remove(tab, "lcd_callback");
	symtab_print(tab);
	symtab_remove(tab, "lcd_init");
	symtab_print(tab);
	symtab_remove(tab, "lcd_bla");
	symtab_print(tab);
	symtab_remove(tab, "lcd_rect");
	symtab_print(tab);
	symtab_remove(tab, "lcd_set_orientation");
	symtab_print(tab);

	int n = 0;
	char *s, *start = NULL;
	for(s = test; *s; ++s)
	{
		if(isalpha(*s))
		{
			if(!start)
			{
				start = s;
			}
		}
		else
		{
			*s = '\0';
			if(start)
			{
				printf("insert: %s\n", start);
				symtab_put(tab, start, n++);
				symtab_print(tab);
				printf("\n\n\n");
				start = NULL;
			}
		}
	}

	printf("N = %d\n", n);

	symtab_print(tab);

	char buf[256] = "lcd_s";
	int rv = symtab_complete(tab, buf);
	printf("%d -> %s\n", rv, buf);

	test_iteration(tab);

	symtab_destroy(tab);
	return 0;
}

