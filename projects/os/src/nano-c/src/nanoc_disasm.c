#ifdef NANOC_DESKTOP
#ifdef NANOC_DEBUG

#include "nanoc_disasm.h"
#include "nanoc_instruction.h"
#include "nanoc_terminal.h"
#include "nanoc_util.h"

#include <stdio.h>

#define R(X) "\033[31;1m" X "\033[0m"

u8r nanoc_instr_print(const u8 *program, size_t offset)
{
	u8r size;
	const u8 *p = program + offset;

	printf(" %5ld | %04lX | " , offset, offset);
	switch(program[offset])
	{
	case NANOC_INSTR_HALT:
		printf(R("HALT") "\n");
		size = 1;
		break;

	case NANOC_INSTR_PUSHI8:
		printf(R("PUSHI8") " %d\n", p[1]);
		size = 2;
		break;

	case NANOC_INSTR_PUSHI16:
		printf(R("PUSHI16") " %d\n", read_16(p + 1));
		size = 3;
		break;

	case NANOC_INSTR_PUSHI32:
		printf(R("PUSHI32") " %d\n", read_32(p + 1));
		size = 5;
		break;

	case NANOC_INSTR_PUSHL:
		printf(R("PUSHL") " %d\n", p[1]);
		size = 2;
		break;

	case NANOC_INSTR_POPL:
		printf(R("POPL") " %d\n", p[1]);
		size = 2;
		break;

	case NANOC_INSTR_JZ:
		printf(R("JZ") " %d\n", read_16(p + 1));
		size = 3;
		break;

	case NANOC_INSTR_JNZ:
		printf(R("JNZ") " %d\n", read_16(p + 1));
		size = 3;
		break;

	case NANOC_INSTR_PJZ:
		printf(R("PJZ") " %d\n", read_16(p + 1));
		size = 3;
		break;

	case NANOC_INSTR_PJNZ:
		printf(R("PJNZ") " %d\n", read_16(p + 1));
		size = 3;
		break;

	case NANOC_INSTR_JMP:
		printf(R("JMP") " %d\n", read_16(p + 1));
		size = 3;
		break;

	case NANOC_INSTR_POP:
		printf(R("POP") "\n");
		size = 1;
		break;

	case NANOC_INSTR_CALL:
		printf(R("CALL") " %d (%d)\n", read_s16(p + 2), p[1]);
		size = 4;
		break;

	case NANOC_INSTR_RET:
		printf(R("RET") "\n");
		size = 1;
		break;

	case NANOC_INSTR_ISP:
		printf(R("ISP") " %d\n", p[1]);
		size = 2;
		break;

	case NANOC_INSTR_INC:
		printf(R("INC") " %d\n", p[1]);
		size = 2;
		break;

	case NANOC_INSTR_DEC:
		printf(R("DEC") " %d\n", p[1]);
		size = 2;
		break;

	case NANOC_INSTR_B_OR:    printf(R("B_OR\n"));    size = 1; break;
	case NANOC_INSTR_B_XOR:   printf(R("B_XOR\n"));   size = 1; break;
	case NANOC_INSTR_B_AND:   printf(R("B_AND\n"));   size = 1; break;
	case NANOC_INSTR_EQ:      printf(R("EQ\n"));      size = 1; break;
	case NANOC_INSTR_NE:      printf(R("NE\n"));      size = 1; break;
	case NANOC_INSTR_LT:      printf(R("LT\n"));      size = 1; break;
	case NANOC_INSTR_GT:      printf(R("GT\n"));      size = 1; break;
	case NANOC_INSTR_LE:      printf(R("LE\n"));      size = 1; break;
	case NANOC_INSTR_GE:      printf(R("GE\n"));      size = 1; break;
	case NANOC_INSTR_SHL:     printf(R("SHL\n"));     size = 1; break;
	case NANOC_INSTR_SHR:     printf(R("SHR\n"));     size = 1; break;
	case NANOC_INSTR_ADD:     printf(R("ADD\n"));     size = 1; break;
	case NANOC_INSTR_SUB:     printf(R("SUB\n"));     size = 1; break;
	case NANOC_INSTR_MUL:     printf(R("MUL\n"));     size = 1; break;
	case NANOC_INSTR_DIV:     printf(R("DIV\n"));     size = 1; break;
	case NANOC_INSTR_MOD:     printf(R("MOD\n"));     size = 1; break;
	case NANOC_INSTR_L_NOT:   printf(R("L_NOT\n"));   size = 1; break;
	case NANOC_INSTR_B_NOT:   printf(R("B_NOT\n"));   size = 1; break;
	case NANOC_INSTR_U_MINUS: printf(R("U_MINUS\n")); size = 1; break;

	default:
		printf(R("INVALID") "\n");
		size = 1;
		break;
	}

	return size;
}

void nanoc_disasm(const u8 *program, size_t len)
{
	size_t offset = 0;
	while(offset < len)
	{
		offset += nanoc_instr_print(program, offset);
	}
}

#endif /* NANOC_DEBUG */
#endif /* NANOC_DESKTOP */
