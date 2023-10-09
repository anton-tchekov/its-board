/**
 * @file    nanoc_address_stack.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC address stack for break and continue
 */

#ifndef __NANOC_ADDRESS_STACK_H__
#define __NANOC_ADDRESS_STACK_H__

#include "types.h"
#include "nanoc_output.h"

typedef struct NANOC_ADDRESS_STACK
{
	u16 *Stack;
	size_t Top;
} NanoC_AddressStack;

static inline void nanoc_address_stack_init(NanoC_AddressStack *as, u16 *stack)
{
	as->Top = 0;
	as->Stack = stack;
}

static inline void nanoc_address_stack_push(
	NanoC_AddressStack *as, NanoC_Address addr)
{
	as->Stack[as->Top++] = addr;
}

void nanoc_address_stack_update(NanoC_AddressStack *as,
	NanoC_Output *output, size_t prev, NanoC_Address addr);

#endif /* __NANOC_ADDRESS_STACK_H__ */
