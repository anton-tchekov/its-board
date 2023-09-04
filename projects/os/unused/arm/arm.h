#ifndef __ARM_H__
#define __ARM_H__

#include <stdint.h>

typedef enum ARM_REGISTER
{
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	FP = R11, /* Frame Pointer */
	R12,
	IP = R12, /* Interprocedural Scratch Register */
	R13,
	SP = R13, /* Stack Pointer */
	R14,
	LR = R14, /* Link Register */
	R15,
	PC = R15  /* Program Counter */
} ARM_Register;

typedef uint32_t ARM_Instruction;

#endif /* __ARM_H__ */
