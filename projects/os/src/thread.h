#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdint.h>

uint32_t getsp(void);
void yield(void);
void thread_init(uint32_t stack, void (*fn)(void));
void threads_init(void);

#endif
