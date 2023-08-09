#include <stdio.h>
#include "symtabl.h"

int main(void)
{
	SymEntry tab;
	symtab_init(&tab);

	symval val = 0;
	int res = symbol_get(&tab, "hou", &val);

	printf("%d %d\n", res, val);

	return 0;
}

