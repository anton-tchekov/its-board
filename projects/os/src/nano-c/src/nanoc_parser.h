/**
 * @file    nanoc_parser.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC parser
 */

#ifndef __NANOC_PARSER_H__
#define __NANOC_PARSER_H__

#include "nanoc_types.h"
#include "nanoc_tokenstream.h"
#include "nanoc_output.h"
#include "nanoc_address_stack.h"
#include "nanoc_map.h"
#include "nanoc_types.h"
#include "nanoc_builtin.h"

#define NANOC_FUNCTION_CAPACITY   32
#define NANOC_VARIABLE_CAPACITY   32
#define NANOC_OPERATOR_STACK_SIZE 32
#define NANOC_ANDOR_STACK_SIZE    16
#define NANOC_MAX_FN_ARGS         16
#define NANOC_BC_BUFFER_SIZE      16

#define NANOC_EXPECT(TYPE, ERROR) \
	do \
	{ \
		if(nanoc_tokenstream_get(&parser->TokenStream, 0)->Type != TYPE) \
		{ \
			NANOC_THROW(ERROR); \
		} \
	} while(0)

#define NANOC_TOKEN(N) \
	nanoc_tokenstream_get(&parser->TokenStream, (N))

#define NANOC_TT(N) \
	(NANOC_TOKEN(N)->Type)

#define NANOC_NEXT() \
	nanoc_tokenstream_next(&parser->TokenStream)

typedef struct NANOC_PARSER
{
	NanoC_MapElement VariableBuffer[NANOC_VARIABLE_CAPACITY];
	NanoC_MapElement FunctionBuffer[NANOC_FUNCTION_CAPACITY];
	u8 OperatorStack[NANOC_OPERATOR_STACK_SIZE];
	u16 AndOrStack[NANOC_ANDOR_STACK_SIZE];
	u16 BreakBuffer[NANOC_BC_BUFFER_SIZE];
	u16 ContinueBuffer[NANOC_BC_BUFFER_SIZE];
	u16 FunctionAddrs[NANOC_FUNCTION_CAPACITY];
	u8 FunctionArgs[NANOC_FUNCTION_CAPACITY];
	NanoC_TokenStream TokenStream;
	NanoC_Output Output;
	NanoC_AddressStack BreakStack;
	NanoC_AddressStack ContinueStack;
	NanoC_Map Variables;
	NanoC_Map Functions;
	const NanoC_ParserBuiltins *Builtins;
	size_t NumLocals;
	size_t BreakNesting;
	size_t ContinueNesting;
	size_t AndOrTop;
	size_t OpTop;
} NanoC_Parser;

void nanoc_parser_init(NanoC_Parser *parser, const char *source,
	char *strings, u8 *output, size_t output_size,
	const NanoC_ParserBuiltins *builtins);

NanoC_Status nanoc_fn_call(NanoC_Parser *parser);
NanoC_Status nanoc_statement(NanoC_Parser *parser);
NanoC_Status nanoc_block(NanoC_Parser *parser);
NanoC_Status nanoc_substmt(NanoC_Parser *parser, NanoC_TokenType end);
NanoC_Status nanoc_int(NanoC_Parser *parser);
NanoC_Status nanoc_function(NanoC_Parser *parser);
NanoC_Status nanoc_file(NanoC_Parser *parser);

#endif /* __NANOC_PARSER_H__ */
