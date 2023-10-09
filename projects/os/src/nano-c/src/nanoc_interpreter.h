/**
 * @file    nanoc_interpreter.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC interpreter
 */

#ifndef __NANOC_INTERPRETER_H__
#define __NANOC_INTERPRETER_H__

#include "nanoc_types.h"
#include "nanoc_status.h"
#include "nanoc_builtin.h"

#define NANOC_OP_STACK_SIZE     32
#define NANOC_CALL_STACK_SIZE  512

NanoC_Status nanoc_interpreter_run(const u8 *program,
	const NanoC_Builtins *builtins, NanoC_Value *rv);

#endif /* __NANOC_INTERPRETER_H__ */
