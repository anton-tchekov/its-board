#include <stdio.h>
#include "symtabl.h"

int main(void)
{
	char test[] = "Radix trees support insertion, deletion, and searching operations. Insertion adds a new string to the trie while trying to minimize the amount of data stored. Deletion removes a string from the trie. Searching operations include (but are not necessarily limited to) exact lookup, find predecessor, find successor, and find all strings with a prefix. All of these operations are O(k) where k is the maximum length of all strings in the set, where length is measured in the quantity of bits equal to the radix of the radix trie. ";


	SymTab *tab;
	tab = symtab_create();

	/*symtab_print(tab);

	symval val;
	int res;

	val = 0;
	res = symtab_get(tab, "houseboat", &val);
	printf("houseboat: %d %d\n", res, val);

	val = 0;
	res = symtab_get(tab, "house", &val);
	printf("house: %d %d\n", res, val);

	val = 0;
	res = symtab_get(tab, "horse", &val);
	printf("horse: %d %d\n", res, val);

	val = 0;
	res = symtab_get(tab, "howl", &val);
	printf("howl: %d %d\n", res, val);

	val = 0;
	res = symtab_get(tab, "hound", &val);
	printf("hound: %d %d\n", res, val);

	printf("\n\n\n");
	printf("inserting: houseboat\n");
	symtab_put(tab, "houseboat", 53);

	printf("\n\n\n");
	printf("inserting: hounddog\n");
	symtab_put(tab, "hounddog", 89);

	printf("\n\n\n");
	printf("inserting: horsegirl\n");
	symtab_put(tab, "horsegirl", 99);

	printf("\n\n\n");
	printf("inserting: houttt\n");
	symtab_put(tab, "houttt", 89);

	printf("\n\n\n");
	printf("inserting: apple\n");
	symtab_put(tab, "apple", 89);

	printf("\n\n\n");
	printf("inserting: applecomputer\n");
	symtab_put(tab, "applecomputer", 89);

	printf("\n\n\n");
	printf("inserting: applecontractor\n");
	symtab_put(tab, "applecontractor", 89);
	printf("\n\n\n");*/


	symtab_put(tab, "lcd_init", 1);
	symtab_put(tab, "lcd_set_orientation", 1);
	symtab_put(tab, "lcd_rect", 1);
	symtab_put(tab, "lcd_callback", 1);
	symtab_put(tab, "lcd_clear", 1);
	symtab_put(tab, "lcd_set_backlight", 1);

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

	char buf[256] = "lcd_set_back";
	int rv = symtab_complete(tab, buf);
	printf("%d -> %s\n", rv, buf);

	symtab_destroy(tab);
	return 0;
}

