#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "symtabl.h"

#define INSERT_TEST_ITERATIONS (1 * 1000 * 1000)

static char *gen_rand_str(void)
{
	int len;
	char *str, *s;

	len = 1 + rand() % 14;
	str = malloc(len + 1);
	s = str;
	do
	{
		*s++ = 'A' + rand() % 25;
	} while(--len);
	*s = '\0';
	return str;
}

typedef struct
{
	char *Label;
	symval Value;
} Mem;

static void insert_test(SymTab *tab)
{
	int i, j, count, rand_idx;
	symval rand_val, read_val;
	char *rand_str;
	Mem *mem;

	i = INSERT_TEST_ITERATIONS;
	count = 0;
	mem = malloc(i * sizeof(*mem));
	do
	{
		rand_str = gen_rand_str();
		printf("insert: %s\n", rand_str);
		rand_val = count;//rand() % 10000000;
		mem[count].Value = rand_val;
		mem[count].Label = rand_str;
		++count;

symtab_print(tab);
		symtab_put(tab, rand_str, rand_val);

		j = 2 * count;
		while(j--)
		{

			rand_idx = rand() % count;
			if(!symtab_get(tab, mem[rand_idx].Label, &read_val))
			{
				printf("ALARM! Wert nicht gespeichert: %d\n", rand_idx);
				exit(1);
			}

			if(read_val != mem[rand_idx].Value)
			{
				printf("ALARM! Falscher Wert gespeichert: @ %d \n"
					"sollte %d sein war aber %d\n"
					"str = %s\n",
					rand_idx,
					mem[rand_idx].Value, read_val,
					mem[rand_idx].Label);

				symtab_print(tab);
				exit(1);
			}
		}
	}
	while(--i);
}

static void with_symtab(void (*callback)(SymTab *tab))
{
	SymTab *tab;
	tab = symtab_create();
	callback(tab);
	symtab_destroy(tab);
}

int main(void)
{
	srand(time(NULL));
	with_symtab(insert_test);
	return 0;
}