#include "nanoc_address_stack.h"

void nanoc_address_stack_update(
	NanoC_AddressStack *as, NanoC_Output *output, u8r prev, u16r addr)
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
