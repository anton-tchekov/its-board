#ifndef __ARM_COND_H__
#define __ARM_COND_H__

#include "arm.h"

typedef enum ARM_CONDITION
{
	COND_EQ = 0x0, /* Equal (Z == 1) */
	COND_NE = 0x1, /* Not Equal (Z == 0) */
	COND_CS = 0x2, /* Carry Set = Unsigned Higher or Same (C == 1) */
	COND_HS = COND_CS,
	COND_CC = 0x3, /* Carry Clear = Unsigned Lower (C == 0) */
	COND_LO = COND_CC,
	COND_MI = 0x4, /* Negative (N == 1) */
	COND_PL = 0x5, /* Positive (N == 0) */
	COND_VS = 0x6, /* Overflow Set (V == 1) */
	COND_VC = 0x7, /* Overflow Clear (V == 0) */
	COND_HI = 0x8, /* Unsigned Higher (C == 1) && (Z == 0) */
	COND_LS = 0x9, /* Unsigned Lower or Same (C == 0) || (Z == 0) */
	COND_GE = 0xA, /* Signed Greater or Equal (N == V) */
	COND_LT = 0xB, /* Signed Less Than (N != V) */
	COND_GT = 0xC, /* Signed Greater Than (Z == 0) && (N == V) */
	COND_LE = 0xD, /* Signed Less or Equal (Z == 1) || (N != V) */
	COND_AL = 0xE, /* Always */
} ARM_Condition;

ARM_Instruction arm_set_cond(ARM_Instruction instr, ARM_Condition cond);

#endif /* __ARM_COND_H__ */
