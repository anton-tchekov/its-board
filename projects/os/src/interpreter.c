#include "nanoc.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(NANOC_ENABLE_INTERPRETER) && NANOC_ENABLE_INTERPRETER != 0

#if 0
void nanoc_disasm(u8 *program, int len)
{
	int ip;
	ip = 0;
	while(ip < len)
	{
		printf("%04d: ", ip);
		switch(program[ip])
		{
		case INSTR_HALT:
			printf("HALT\n");
			break;

		case INSTR_PUSHI8:
			printf("PUSHI8 %d\n", program[ip + 1]);
			++ip;
			break;

		case INSTR_PUSHI16:
			printf("PUSHI16 %d\n", _read_16(program + ip + 1));
			ip += 2;
			break;

		case INSTR_PUSHL:
			++ip;
			printf("PUSHL %d\n", program[ip]);
			break;

		case INSTR_POPL:
			++ip;
			printf("POPL %d\n", program[ip]);
			break;

		case INSTR_PUSHA8:
			printf("PUSHA8\n");
			break;

		case INSTR_PUSHA16:
			printf("PUSHA16\n");
			break;

		case INSTR_PUSHA32:
			printf("PUSHA32\n");
			break;

		case INSTR_POPA8:
			printf("POPA8\n");
			break;

		case INSTR_POPA16:
			printf("POPA16\n");
			break;

		case INSTR_POPA32:
			printf("POPA32\n");
			break;

		case INSTR_DUP:
			printf("DUP\n");
			break;

		case INSTR_POP:
			printf("POP\n");
			break;

		case INSTR_JZ:
			printf("JZ %d\n", _read_16(program + ip + 1));
			ip += 2;
			break;

		case INSTR_JNZ:
			printf("JNZ %d\n", _read_16(program + ip + 1));
			ip += 2;
			break;

		case INSTR_JMP:
			printf("JMP %d\n", _read_16(program + ip + 1));
			ip += 2;
			break;

		case INSTR_CALL:
			printf("CALL %d %d\n", program[ip + 1], (i16)_read_16(program + ip + 2));
			ip += 3;
			break;

		case INSTR_RET:
			printf("RET\n");
			break;

		case INSTR_DSP:
			++ip;
			printf("DSP %d\n", program[ip]);
			break;

		case INSTR_U_MINUS:
			printf("U_MINUS\n");
			break;

		case INSTR_L_NOT:
			printf("L_NOT\n");
			break;

		case INSTR_B_NOT:
			printf("B_NOT\n");
			break;

		case INSTR_L_OR:
			printf("L_OR\n");
			break;

		case INSTR_L_AND:
			printf("L_AND\n");
			break;

		case INSTR_B_OR:
			printf("B_OR\n");
			break;

		case INSTR_B_XOR:
			printf("B_XOR\n");
			break;

		case INSTR_B_AND:
			printf("B_AND\n");
			break;

		case INSTR_EQ:
			printf("EQ\n");
			break;

		case INSTR_NE:
			printf("NE\n");
			break;

		case INSTR_LT:
			printf("LT\n");
			break;

		case INSTR_GT:
			printf("GT\n");
			break;

		case INSTR_LE:
			printf("LE\n");
			break;

		case INSTR_GE:
			printf("GE\n");
			break;

		case INSTR_B_SHL:
			printf("B_SHL\n");
			break;

		case INSTR_B_SHR:
			printf("B_SHR\n");
			break;

		case INSTR_ADD:
			printf("ADD\n");
			break;

		case INSTR_SUB:
			printf("SUB\n");
			break;

		case INSTR_MUL:
			printf("MUL\n");
			break;

		case INSTR_DIV:
			printf("DIV\n");
			break;

		case INSTR_MOD:
			printf("MOD\n");
			break;

		default:
			printf("INVALID\n");
			break;
		}

		++ip;
	}
}
#endif

#define INTERPRETER_STACK_UNDERFLOW 1
#define INTERPRETER_STACK_OVERFLOW 2
#define INTERPRETER_INV_INSTR 3

/** Check for operator stack underflow */
#define CHECK_UNDERFLOW(N) \
	if(op < (N)) \
	{ \
		return INTERPRETER_STACK_UNDERFLOW; \
	}

/** Check for operator stack overflow */
#define CHECK_OVERFLOW(N) \
	if(op > INTERPRETER_OP_STACK_SIZE - (N)) \
	{ \
		return INTERPRETER_STACK_OVERFLOW; \
	}

#define BINARY_OP(C) \
	{ \
		register int A, B; \
		CHECK_UNDERFLOW(2); \
		op -= 1; \
		A = op_stack[op - 1]; \
		B = op_stack[op]; \
		op_stack[op - 1] = (C); \
		ip += 1; \
	}

#define UNARY_OP(C) \
	{ \
		register int A; \
		CHECK_UNDERFLOW(1); \
		A = op_stack[op - 1]; \
		op_stack[op - 1] = (C); \
		ip += 1; \
	}

int nanoc_run(NanoC *n, u8 *program, u8 *data)
{
	Interpreter *i = &n->Interpreter;

	int count = 0;

	i->Heap = data;

	i->SP = 0;
	i->FP = 0;

	int op_stack[128];

	register int ip = 0;
	register int op = 0;

	while(program[ip] != INSTR_HALT)
	{
		++count;
		switch(program[ip])
		{
		case INSTR_PUSHI8:
			if(op >= NANOC_OP_STACK_SIZE - 1)
			{
				TRACE(ERROR_STACK_OVERFLOW);
			}

			++ip;
			op_stack[op++] = program[ip++];
			break;

		case INSTR_PUSHI16:
			if(op >= NANOC_OP_STACK_SIZE - 1)
			{
				TRACE(ERROR_STACK_OVERFLOW);
			}

			++ip;
			op_stack[op++] = _read_16(program + ip);
			ip += 2;
			break;

		case INSTR_PUSHL:
			if(op >= NANOC_OP_STACK_SIZE - 1)
			{
				TRACE(ERROR_STACK_OVERFLOW);
			}

			++ip;
			op_stack[op++] = i->CallStack[i->FP + program[ip++]];
			break;

		case INSTR_POPL:
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			i->CallStack[i->FP + program[ip++]] = op_stack[--op];
			break;

		case INSTR_PUSHA8:
		{
			int addr;
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			addr = op_stack[op - 1];
			if(addr < 0 && addr >= NANOC_HEAP_SIZE)
			{
				TRACE(ERROR_INV_MEM_ACCESS);
			}

			op_stack[op - 1] = i->Heap[addr];
			break;
		}

		case INSTR_PUSHA16:
		{
			int addr;
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			addr = op_stack[op - 1];
			if(addr < 0 && addr >= NANOC_HEAP_SIZE)
			{
				TRACE(ERROR_INV_MEM_ACCESS);
			}

			op_stack[op - 1] = _read_16(i->Heap + addr);
			break;
		}

		case INSTR_POPA8:
		{
			int addr;
			if(op < 2)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			addr = op_stack[op - 2];
			if(addr < 0 || addr >= NANOC_HEAP_SIZE)
			{
				TRACE(ERROR_INV_MEM_ACCESS);
			}

			i->Heap[addr] = op_stack[op - 1];
			--op;
			break;
		}

		case INSTR_POPA16:
		{
			int addr;
			if(op < 2)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			addr = op_stack[op - 2];
			if(addr < 0 || addr >= NANOC_HEAP_SIZE)
			{
				TRACE(ERROR_INV_MEM_ACCESS);
			}

			_write_16(i->Heap + addr, op_stack[op - 1]);
			--op;
			break;
		}

		case INSTR_DUP:
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			op_stack[op] = op_stack[op - 1];
			++op;
			break;

		case INSTR_JZ:
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			if(!op_stack[--op])
			{
				ip = _read_16(program + ip);
				continue;
			}

			ip += 2;
			break;

		case INSTR_JNZ:
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			if(op_stack[--op])
			{
				ip = _read_16(program + ip);
				continue;
			}

			ip += 2;
			break;

		case INSTR_JMP:
			++ip;
			ip = _read_16(program + ip);
			continue;

		case INSTR_CALL:
		{
			i16 func;
			int j, args;
			++ip;
			args = program[ip];
			++ip;
			func = _read_16(program + ip);
			ip += 2;
			op -= args;
			if(op < 0)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			for(j = 0; j < args; ++j)
			{
				i->CallStack[i->SP + 2 + j] = op_stack[op + j];
			}

			if(func < 0)
			{
				/* native function */
				int ret;
				func = -func - 1;
				ret = i->Functions[func]((int *)(i->CallStack + i->SP + 2), i->Heap);
				if(op >= NANOC_OP_STACK_SIZE - 1)
				{
					TRACE(ERROR_STACK_OVERFLOW);
				}

				op_stack[op++] = ret;
				break;
			}

			i->CallStack[i->SP++] = ip;
			i->CallStack[i->SP++] = i->FP;
			i->FP = i->SP;
			ip = func;
			break;
		}

		case INSTR_RET:
			i->SP = i->FP;
			i->FP = i->CallStack[--i->SP];
			ip = i->CallStack[--i->SP];
			break;

		case INSTR_DSP:
			++ip;
			i->SP += program[ip];
			++ip;
			break;

		case INSTR_POP:
			if(op < 1)
			{
				TRACE(ERROR_STACK_UNDERFLOW);
			}

			++ip;
			--op;
			break;

		case INSTR_U_MINUS: UNARY_OP(-A);       break;
		case INSTR_L_NOT:   UNARY_OP(!A);       break;
		case INSTR_B_NOT:   UNARY_OP(~A);       break;
		case INSTR_L_OR:    BINARY_OP(A || B);  break;
		case INSTR_L_AND:   BINARY_OP(A && B);  break;
		case INSTR_B_OR:    BINARY_OP(A | B);   break;
		case INSTR_B_XOR:   BINARY_OP(A ^ B);   break;
		case INSTR_B_AND:   BINARY_OP(A & B);   break;
		case INSTR_EQ:      BINARY_OP(A == B);  break;
		case INSTR_NE:      BINARY_OP(A != B);  break;
		case INSTR_LT:      BINARY_OP(A < B);   break;
		case INSTR_GT:      BINARY_OP(A > B);   break;
		case INSTR_LE:      BINARY_OP(A <= B);  break;
		case INSTR_GE:      BINARY_OP(A >= B);  break;
		case INSTR_B_SHL:   BINARY_OP(A << B);  break;
		case INSTR_B_SHR:   BINARY_OP(A >> B);  break;
		case INSTR_ADD:     BINARY_OP(A + B);   break;
		case INSTR_SUB:     BINARY_OP(A - B);   break;
		case INSTR_MUL:     BINARY_OP(A * B);   break;
		case INSTR_DIV:     BINARY_OP(A / B);   break;
		case INSTR_MOD:     BINARY_OP(A % B);   break;
		default:
			return INTERPRETER_INV_INSTR;
		}
	}

	//printf("INSTR COUNT = %d\n", count);
	return 0;
}

#endif

