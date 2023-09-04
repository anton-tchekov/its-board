#include "arm.h"
#include "arm_cond.h"

ARM_Instruction parse_instruction(const char *ident)
{
	const uint8_t opcode_cond[] =
	{
		'E', 'Q', COND_EQ,
		'N', 'E', COND_NE,
		'C', 'S', COND_CS,
		'H', 'S', COND_HS,
		'C', 'C', COND_CC,
		'L', 'O', COND_LO,
		'M', 'I', COND_MI,
		'P', 'L', COND_PL,
		'V', 'S', COND_VS,
		'V', 'C', COND_VC,
		'H', 'I', COND_HI,
		'L', 'S', COND_LS,
		'G', 'E', COND_GE,
		'L', 'T', COND_LT,
		'G', 'T', COND_GT,
		'L', 'E', COND_LE,
		'A', 'L', COND_AL,
		'\0'
	};

	const char *instr =
		"STR"
		"LDR"
		"PUSH"
		"POP"
		"LSL"
		"LSR"
		"ASR"
		"ROR"
		"AND"
		"EOR"
		"SUB"
		"RSB"
		"ADD"
		"ADC"
		"SBC"
		"RSC"
		"TST"
		"TEQ"
		"CMP"
		"CMN"
		"ORR"
		"MOV"
		"BIC"
		"MVN";

}