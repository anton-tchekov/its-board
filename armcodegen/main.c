#include <stdio.h>
#include <ctype.h>
#include "symtabl.h"
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

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

int contonlyascii(char *buffer)
{
	while(isalpha(*buffer))
	{
		*buffer = tolower(*buffer);
		++buffer;
	}

	if(*buffer == '\n')
	{
		*buffer = '\0';
	}

	return !*buffer;
}

void stresstest(void)
{
	struct timeval tv1, tv2;

	int n = 0;
	char **linbuf = malloc(16 * 1024 * 1024);

	SymTab *tab;
	tab = symtab_create();
	FILE* filePointer;
	int bufferLength = 255;
	char buffer[bufferLength];
	filePointer = fopen("words.txt", "r");

	while(fgets(buffer, bufferLength, filePointer)) {
		if(contonlyascii(buffer))
		{
			linbuf[n] = malloc(strlen(buffer) + 1);
			strcpy(linbuf[n], buffer);
			++n;
			symtab_put(tab, buffer, rand());
		}
	}

	//symtab_print(tab);

printf("\n\n----------- LOOKUP EFFICIENCY TEST ----------\n\n");

printf("\n\n----------- RADIX TREE IMPLEMENTATION ----------\n\n");

char *word = "antidisestablishmentarianism";

double s0, s1, ratio;
gettimeofday(&tv1, NULL);
	int rv = symtab_exists(tab, word);
gettimeofday(&tv2, NULL);

printf("exists = %d\n", rv);

s0 = (double) (tv2.tv_usec - tv1.tv_usec) / 1000;
printf ("Total time = %g ms\n",
         s0);


gettimeofday(&tv1, NULL);

printf("\n\n----------- ARRAY IMPLEMENTATION ----------\n\n");

	rv  = 0;
	for(int i =0; i < n; ++i)
	{
		if(!strcmp(linbuf[i], word))
		{
			rv = 1;
			break;
		}
	}

gettimeofday(&tv2, NULL);

printf("exists = %d\n", rv);

s1 = (double) (tv2.tv_usec - tv1.tv_usec) / 1000;
printf ("Total time = %g ms\n",
         s1);

printf("N = %d\n", n);
printf("RATIO = %g times faster\n", s1 / s0);

	fclose(filePointer);
	symtab_destroy(tab);


}

int main(void)
{

	//stresstest();

	SymTab *tab;
	tab = symtab_create();

	symtab_put(tab, "test", 1);
	symtab_put(tab, "team", 2);
	symtab_put(tab, "toast", 3);
	symtab_put(tab, "a", 5);

	symval val = 0xFF;
	symtab_get(tab, "", &val);

	printf("symtab_get(\"\") = %d", val);

	symtab_print(tab);

/*
	char test[] = "Radix trees support insertion, deletion, and searching operations. Insertion adds a new string to the trie while trying to minimize the amount of data stored. Deletion removes a string from the trie. Searching operations include (but are not necessarily limited to) exact lookup, find predecessor, find successor, and find all strings with a prefix. All of these operations are O(k) where k is the maximum length of all strings in the set, where length is measured in the quantity of bits equal to the radix of the radix trie. ";
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
*/
	//test_iteration(tab);

	symtab_destroy(tab);

	return 0;
}

