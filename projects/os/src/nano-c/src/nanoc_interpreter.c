#include "nanoc_interpreter.h"
#include "nanoc_instruction.h"
#include "nanoc_debug.h"
#include "nanoc_util.h"
#include "nanoc_disasm.h"

#define CHECK_UNDERFLOW(N) \
	if(op < (N)) \
	{ \
		NANOC_THROW(NANOC_ERROR_UNDERFLOW); \
	}

#define CHECK_OVERFLOW(N) \
	if(op > NANOC_OP_STACK_SIZE - (N)) \
	{ \
		NANOC_THROW(NANOC_ERROR_OVERFLOW); \
	}

#define BINARY_OP(C) \
	{ \
		NanoC_Value A, B; \
		CHECK_UNDERFLOW(2); \
		op -= 1; \
		A = op_stack[op - 1]; \
		B = op_stack[op]; \
		op_stack[op - 1] = (C); \
		ip += 1; \
	}

#define BINARY_OP_DIV(C) \
	{ \
		NanoC_Value A, B; \
		CHECK_UNDERFLOW(2); \
		op -= 1; \
		A = op_stack[op - 1]; \
		B = op_stack[op]; \
		if(B == 0) \
		{ \
			NANOC_THROW(NANOC_ERROR_DIVISION_BY_ZERO); \
		} \
		op_stack[op - 1] = (C); \
		ip += 1; \
	}

#define UNARY_OP(C) \
	{ \
		NanoC_Value A; \
		CHECK_UNDERFLOW(1); \
		A = op_stack[op - 1]; \
		op_stack[op - 1] = (C); \
		ip += 1; \
	}

NanoC_Status nanoc_interpreter_run(const u8 *program,
	const NanoC_Builtins *builtins, NanoC_Value *rv)
{
	NanoC_Value op_stack[NANOC_OP_STACK_SIZE];
	NanoC_Value call_stack[NANOC_CALL_STACK_SIZE];

	u8r op = 0;
	u16r sp = 0;
	u16r fp = 0;
	u16r ip = 0;

	for(;;)
	{
		switch(program[ip])
		{
		case NANOC_INSTR_HALT:
			*rv = op_stack[0];
			return NANOC_STATUS_SUCCESS;

		case NANOC_INSTR_PUSHI8:
			CHECK_OVERFLOW(1);
			op_stack[op] = program[ip + 1];
			op += 1;
			ip += 2;
			break;

		case NANOC_INSTR_PUSHI16:
			CHECK_OVERFLOW(1);
			op_stack[op] = nanoc_read_16(program + ip + 1);
			op += 1;
			ip += 3;
			break;

		case NANOC_INSTR_PUSHI32:
			CHECK_OVERFLOW(1);
			op_stack[op] = nanoc_read_32(program + ip + 1);
			op += 1;
			ip += 5;
			break;

		case NANOC_INSTR_PUSHL:
			CHECK_OVERFLOW(1);
			op_stack[op] = call_stack[fp + program[ip + 1]];
			op += 1;
			ip += 2;
			break;

		case NANOC_INSTR_POPL:
			CHECK_UNDERFLOW(1);
			op -= 1;
			call_stack[fp + program[ip + 1]] = op_stack[op];
			ip += 2;
			break;

		case NANOC_INSTR_JZ:
			CHECK_UNDERFLOW(1);
			op -= 1;
			if(!op_stack[op])
			{
				ip = nanoc_read_16(program + ip + 1);
				break;
			}

			ip += 3;
			break;

		case NANOC_INSTR_JNZ:
			CHECK_UNDERFLOW(1);
			op -= 1;
			if(op_stack[op])
			{
				ip = nanoc_read_16(program + ip + 1);
				break;
			}

			ip += 3;
			break;

		case NANOC_INSTR_PJZ: /* L_AND */
			CHECK_UNDERFLOW(1);
			if(!op_stack[op - 1])
			{
				/* If first condition is false, skip */
				ip = nanoc_read_16(program + ip + 1);
				break;
			}

			/* Only the second condition is relevant for result */
			op -= 1;
			ip += 3;
			break;

		case NANOC_INSTR_PJNZ: /* L_OR */
			CHECK_UNDERFLOW(1);
			if(op_stack[op - 1])
			{
				/* If first condition is true, skip */
				ip = nanoc_read_16(program + ip + 1);
				break;
			}

			/* Only the second condition is relevant for result */
			op -= 1;
			ip += 3;
			break;

		case NANOC_INSTR_JMP:
			ip = nanoc_read_16(program + ip + 1);
			break;

		case NANOC_INSTR_CALL:
		{
			u8r i;
			u8r args = program[ip + 1];
			i16r addr = nanoc_read_s16(program + ip + 2);
			ip += 4;
			CHECK_UNDERFLOW(args);
			op -= args;
			for(i = 0; i < args; ++i)
			{
				call_stack[sp + 2 + i] = op_stack[op + i];
			}

			if(addr < 0)
			{
				/* TODO: Possible out ouf bounds funcitons call? */
				CHECK_OVERFLOW(1);
				op_stack[op] = builtins->Functions[-addr - 1]
					(args, call_stack + sp + 2);
				op += 1;
			}
			else
			{
				call_stack[sp] = ip;
				sp += 1;
				call_stack[sp] = fp;
				sp += 1;
				fp = sp;
				ip = addr;
			}
			break;
		}

		case NANOC_INSTR_RET:
			if(fp < 2)
			{
				NANOC_THROW(NANOC_ERROR_UNDERFLOW);
			}

			sp = fp;
			sp -= 1;
			fp = call_stack[sp];
			sp -= 1;
			ip = call_stack[sp];
			break;

		case NANOC_INSTR_ISP:
			sp += program[ip + 1];
			ip += 2;
			break;

		case NANOC_INSTR_POP:
			CHECK_UNDERFLOW(1);
			op -= 1;
			ip += 1;
			break;

		case NANOC_INSTR_INC:
			++call_stack[fp + program[ip + 1]];
			ip += 2;
			break;

		case NANOC_INSTR_DEC:
			--call_stack[fp + program[ip + 1]];
			ip += 2;
			break;

		case NANOC_INSTR_B_OR:    BINARY_OP(A | B);      break;
		case NANOC_INSTR_B_XOR:   BINARY_OP(A ^ B);      break;
		case NANOC_INSTR_B_AND:   BINARY_OP(A & B);      break;
		case NANOC_INSTR_EQ:      BINARY_OP(A == B);     break;
		case NANOC_INSTR_NE:      BINARY_OP(A != B);     break;
		case NANOC_INSTR_LT:      BINARY_OP(A < B);      break;
		case NANOC_INSTR_GT:      BINARY_OP(A > B);      break;
		case NANOC_INSTR_LE:      BINARY_OP(A <= B);     break;
		case NANOC_INSTR_GE:      BINARY_OP(A >= B);     break;
		case NANOC_INSTR_SHL:     BINARY_OP(A << B);     break;
		case NANOC_INSTR_SHR:     BINARY_OP(A >> B);     break;
		case NANOC_INSTR_ADD:     BINARY_OP(A + B);      break;
		case NANOC_INSTR_SUB:     BINARY_OP(A - B);      break;
		case NANOC_INSTR_MUL:     BINARY_OP(A * B);      break;
		case NANOC_INSTR_DIV:     BINARY_OP_DIV(A / B);  break;
		case NANOC_INSTR_MOD:     BINARY_OP_DIV(A % B);  break;
		case NANOC_INSTR_L_NOT:   UNARY_OP(!A);          break;
		case NANOC_INSTR_B_NOT:   UNARY_OP(~A);          break;
		case NANOC_INSTR_U_MINUS: UNARY_OP(-A);          break;

		default:
			NANOC_THROW(NANOC_ERROR_INVALID_INSTRUCTION);
		}
	}

	nanoc_unreachable();
	return NANOC_STATUS_SUCCESS;
}
