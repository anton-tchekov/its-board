#include "thread.h"
#include <string.h>

#define MAX_THREADS 4

typedef struct
{
	int Cur;
	int Cnt;
	int Sps[MAX_THREADS];
} Threads;

Threads threads;

void threads_init(void)
{
	memset(&threads, 0, sizeof(threads));
}

void thread_init(uint32_t stack, void (*fn)(void))
{
	threads.Sps[threads.Cnt++] = stack;
	((uint32_t *)stack)[-1] = (uint32_t)fn;
	setsp(stack);
	fn();
}
