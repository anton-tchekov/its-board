#ifndef __NANOC_INTERPRETER_H__
#define __NANOC_INTERPRETER_H__

#include "nanoc_types.h"
#include "nanoc_status.h"
#include "nanoc_builtin.h"

#define NANOC_OP_STACK_SIZE     32
#define NANOC_CALL_STACK_SIZE  512

NanoC_Status nanoc_interpreter_run(const u8 *program,
	const NanoC_Builtins *builtins, size_t *rv);

#endif /* __NANOC_INTERPRETER_H__ */
