#ifndef __NANOC_ADDRESS_STACK_H__
#define __NANOC_ADDRESS_STACK_H__

typedef struct NANOC_ADDRESS_STACK
{
	u8 Top;
	u16 *Stack;
} NanoC_AddressStack;

static inline void nanoc_address_stack_init(NanoC_AddressStack *as)
{
	as->Top = 0;
}

#endif /* __NANOC_ADDRESS_STACK_H__ */