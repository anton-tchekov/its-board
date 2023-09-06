#ifndef __NANOC_PARSER_H__
#define __NANOC_PARSER_H__

#include "types.h"
#include "nanoc_tokenstream.h"
#include "nanoc_output.h"
#include "nanoc_address_stack.h"
#include "nanoc_map.h"
#include "nanoc_types.h"

#define NANOC_VARIABLE_CAPACITY   32
#define NANOC_OPERATOR_STACK_SIZE 32
#define NANOC_ANDOR_STACK_SIZE    16
#define NANOC_MAX_FN_ARGS         16
#define NANOC_BC_BUFFER_SIZE      16

#define EXPECT(TYPE, ERROR) \
	do \
	{ \
		if(nanoc_tokenstream_get(&parser->TokenStream, 0)->Type != TYPE) \
		{ \
			THROW(ERROR); \
		} \
	} while(0)

#define EXPECT_EQ(A, B, ERROR) \
	do \
	{ \
		if((A) != (B)) \
		{ \
			THROW(ERROR); \
		} \
	} while(0)

#define TOKEN(N) \
	nanoc_tokenstream_get(&parser->TokenStream, (N))

#define TT(N) \
	(TOKEN(N)->Type)

#define NEXT() \
	nanoc_tokenstream_next(&parser->TokenStream)

typedef struct NANOC_PARSER
{
	NanoC_MapElement VariableBuffer[NANOC_VARIABLE_CAPACITY];
	u8 OperatorStack[NANOC_OPERATOR_STACK_SIZE];
	u16 AndOrStack[NANOC_ANDOR_STACK_SIZE];
	u16 BreakBuffer[NANOC_BC_BUFFER_SIZE];
	u16 ContinueBuffer[NANOC_BC_BUFFER_SIZE];
	NanoC_TokenStream TokenStream;
	NanoC_Output Output;
	NanoC_AddressStack BreakStack;
	NanoC_AddressStack ContinueStack;
	NanoC_Map Variables;
	u16 FuncLocalsIdx;
	u8 BreakNesting;
	u8 ContinueNesting;
	u8 AndOrTop;
	u8 OpTop;
} NanoC_Parser;

void nanoc_parser_init(NanoC_Parser *parser, const char *source,
	u8 *output, size_t output_size);

NanoC_Status nanoc_fn_call(NanoC_Parser *parser);
NanoC_Status nanoc_statement(NanoC_Parser *parser);
NanoC_Status nanoc_block(NanoC_Parser *parser);
NanoC_Status nanoc_substmt(NanoC_Parser *parser, NanoC_TokenType end);
NanoC_Status nanoc_let(NanoC_Parser *parser);

#endif /* __NANOC_PARSER_H__ */
