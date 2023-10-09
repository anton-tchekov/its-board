/**
 * @file    nanoc_address_stack.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_address_stack.h"

void nanoc_address_stack_update(
	NanoC_AddressStack *as, NanoC_Output *output, u8r prev, NanoC_Address addr)
{
	u8r top = as->Top;
	u16 *stack = as->Stack;
	while(top > prev)
	{
		--top;
		nanoc_output_emit16_at(output, stack[top], addr);
	}

	as->Top = top;
}
