#ifndef __NANOC_INTERPRETER_H__
#define __NANOC_INTERPRETER_H__

#include "types.h"
#include "nanoc_builtin.h"

#define NANOC_OP_STACK_SIZE     32
#define NANOC_CALL_STACK_SIZE  512

typedef enum NANOC_INTERPRETER_STATUS
{
	NANOC_INTERPRETER_SUCCESS,
	NANOC_INTERPRETER_STACK_UNDERFLOW,
	NANOC_INTERPRETER_STACK_OVERFLOW,
	NANOC_INTERPRETER_DIVISION_BY_ZERO,
	NANOC_INTERPRETER_INVALID_INSTRUCTION
} NanoC_InterpreterStatus;

u8r nanoc_interpreter_run(const u8 *program, NanoC_Builtins *builtins);
const char *nanoc_interpreter_status_message(u8r status_code);

#endif /* __NANOC_INTERPRETER_H__ */
