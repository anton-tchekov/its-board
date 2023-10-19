/**
 * @file    nanoc_instruction.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC interpreter instruction
 */

#ifndef __NANOC_INSTRUCTION_H__
#define __NANOC_INSTRUCTION_H__

typedef enum NANOC_OPCODE
{
	NANOC_INSTR_HALT,
	NANOC_INSTR_PUSHI8,
	NANOC_INSTR_PUSHI16,
	NANOC_INSTR_PUSHI32,
	NANOC_INSTR_PUSHI64,
	NANOC_INSTR_PUSHL,
	NANOC_INSTR_POPL,
	NANOC_INSTR_JZ,
	NANOC_INSTR_JNZ,
	NANOC_INSTR_PJZ,
	NANOC_INSTR_PJNZ,
	NANOC_INSTR_JMP,
	NANOC_INSTR_CALL,
	NANOC_INSTR_RET,
	NANOC_INSTR_ISP,
	NANOC_INSTR_POP,
	NANOC_INSTR_INC,
	NANOC_INSTR_DEC,

	NANOC_INSTR_OP_START,
	NANOC_INSTR_B_OR = NANOC_INSTR_OP_START,
	NANOC_INSTR_B_XOR,
	NANOC_INSTR_B_AND,
	NANOC_INSTR_EQ,
	NANOC_INSTR_NE,
	NANOC_INSTR_LT,
	NANOC_INSTR_GT,
	NANOC_INSTR_LE,
	NANOC_INSTR_GE,
	NANOC_INSTR_SHL,
	NANOC_INSTR_SHR,
	NANOC_INSTR_ADD,
	NANOC_INSTR_SUB,
	NANOC_INSTR_MUL,
	NANOC_INSTR_DIV,
	NANOC_INSTR_MOD,
	NANOC_INSTR_L_NOT,
	NANOC_INSTR_B_NOT,
	NANOC_INSTR_U_MINUS,
	NANOC_INSTR_OP_END = NANOC_INSTR_U_MINUS
} NanoC_Opcode;

#endif /* __NANOC_INSTRUCTION_H__ */
