#include "nanoc_address_stack.h"

void nanoc_address_stack_update(NanoC_AddressStack *as,
	NanoC_Output *output, size_t prev, NanoC_Address addr)
{
	size_t top = as->Top;
	u16 *stack = as->Stack;
	while(top > prev)
	{
		--top;
		nanoc_output_emit_addr_at(output, stack[top], addr);
	}

	as->Top = prev;
}
