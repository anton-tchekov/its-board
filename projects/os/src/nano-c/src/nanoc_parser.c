#include "nanoc_parser.h"
#include "nanoc_loop.h"
#include "nanoc_if.h"
#include "nanoc_instruction.h"
#include "nanoc_status.h"
#include "nanoc_expression.h"

static u8r insert_var(NanoC_Parser *parser, NanoC_Token *token, size_t *idx)
{
	u8r ret;

	if(token->Type != NANOC_TT_IDENTIFIER)
	{
		THROW(NANOC_ERROR_EXPECTED_IDENTIFIER);
	}

	ret = nanoc_map_insert(&parser->Variables,
		token->Ptr, token->Length, idx);

	if(ret == NANOC_ERROR_STACK_OVERFLOW)
	{
		THROW(NANOC_ERROR_TOO_MANY_VARIABLES);
	}
	else if(ret == NANOC_ERROR_DUPLICATE_MAP_ELEMENT)
	{
		THROW(NANOC_ERROR_VARIABLE_REDEFINITION);
	}

	return NANOC_STATUS_SUCCESS;
}

void nanoc_parser_init(NanoC_Parser *parser, const char *source,
	u8 *output, size_t output_size)
{
	nanoc_tokenstream_init(&parser->TokenStream, source);
	nanoc_output_init(&parser->Output, output, output_size);
	nanoc_map_init(&parser->Variables, parser->VariableBuffer,
		NANOC_VARIABLE_CAPACITY);
	parser->OpTop = 0;
	parser->AndOrTop = 0;
	parser->BreakNesting = 0;
	parser->ContinueNesting = 0;
	nanoc_address_stack_init(&parser->BreakStack);
	nanoc_address_stack_init(&parser->ContinueStack);
}

u8r nanoc_block_inner(NanoC_Parser *parser)
{
	NEXT();
	while(TT(0) != NANOC_TT_R_BRACE)
	{
		PROPAGATE(nanoc_statement(parser));
		NEXT();
	}

	return NANOC_STATUS_SUCCESS;
}

u8r nanoc_block(NanoC_Parser *parser)
{
	EXPECT(NANOC_TT_L_BRACE, NANOC_ERROR_EXPECTED_L_BRACE);
	return nanoc_block_inner(parser);
}

u8r nanoc_fn_call(NanoC_Parser *parser)
{
	u8r arg_cnt = 0;
	NEXT();
	NEXT();
	if(TT(0) != NANOC_TT_R_PAREN)
	{
		for(;;)
		{
			u8r tt;
			PROPAGATE(nanoc_expression(parser));
			++arg_cnt;
			if(arg_cnt >= NANOC_MAX_FN_ARGS)
			{
				THROW(NANOC_ERROR_TOO_MANY_FN_ARGS);
			}

			tt = TT(0);
			if(tt == NANOC_TT_R_PAREN)
			{
				break;
			}
			else if(tt == NANOC_TT_COMMA)
			{
				NEXT();
			}
			else
			{
				THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
			}
		}
	}

#if 0
	if(arg_cnt != n->Parser.FN_Args[i])
	{
		THROW(ERROR_FN_NUM_ARGS);
	}
#endif

	nanoc_output_emit2(&parser->Output, NANOC_INSTR_CALL, arg_cnt);
	nanoc_output_emit16(&parser->Output, -1);
	return NANOC_STATUS_SUCCESS;
}

static u8r nanoc_assign(NanoC_Parser *parser, u8r op, size_t idx)
{
	NEXT();
	if(op)
	{
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHL, idx);
	}

	PROPAGATE(nanoc_expression(parser));
	if(op)
	{
		nanoc_output_emit(&parser->Output, op);
	}

	nanoc_output_emit2(&parser->Output, NANOC_INSTR_POPL, idx);
	return NANOC_STATUS_SUCCESS;
}

static u8r is_assign(u8r tt)
{
	return tt >= NANOC_TT_ASSIGN_START && tt <= NANOC_TT_ASSIGN_END;
}

#define ASSIGN_LUT(X) ((X) - NANOC_TT_ASSIGN_START)

static u8r assign_instr(u8r tt)
{
	static const u8 lut[] =
	{
		[ ASSIGN_LUT(NANOC_TT_ASSIGN) ]       = 0,
		[ ASSIGN_LUT(NANOC_TT_ADD_ASSIGN) ]   = NANOC_INSTR_ADD,
		[ ASSIGN_LUT(NANOC_TT_SUB_ASSIGN) ]   = NANOC_INSTR_SUB,
		[ ASSIGN_LUT(NANOC_TT_MUL_ASSIGN) ]   = NANOC_INSTR_MUL,
		[ ASSIGN_LUT(NANOC_TT_DIV_ASSIGN) ]   = NANOC_INSTR_DIV,
		[ ASSIGN_LUT(NANOC_TT_MOD_ASSIGN) ]   = NANOC_INSTR_MOD,
		[ ASSIGN_LUT(NANOC_TT_SHL_ASSIGN) ]   = NANOC_INSTR_SHL,
		[ ASSIGN_LUT(NANOC_TT_SHR_ASSIGN) ]   = NANOC_INSTR_SHR,
		[ ASSIGN_LUT(NANOC_TT_B_OR_ASSIGN) ]  = NANOC_INSTR_B_OR,
		[ ASSIGN_LUT(NANOC_TT_B_AND_ASSIGN) ] = NANOC_INSTR_B_AND,
		[ ASSIGN_LUT(NANOC_TT_B_NOT_ASSIGN) ] = NANOC_INSTR_B_NOT,
		[ ASSIGN_LUT(NANOC_TT_XOR_ASSIGN) ]   = NANOC_INSTR_B_XOR,
	};

	return lut[ASSIGN_LUT(tt)];
}

static u8r nanoc_action(NanoC_Parser *parser)
{
	PROPAGATE(nanoc_fn_call(parser));
	nanoc_output_emit(&parser->Output, NANOC_INSTR_POP);
	NEXT();
	EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	return NANOC_STATUS_SUCCESS;
}

static u8r nanoc_identifier(NanoC_Parser *parser)
{
	NanoC_Token *token = TOKEN(0);
	u8r tt = TT(1);
	if(tt == NANOC_TT_L_PAREN)
	{
		return nanoc_action(parser);
	}
	else if(is_assign(tt))
	{
		size_t local;
		if(!nanoc_map_find(&parser->Variables,
			token->Ptr, token->Length, &local))
		{
			THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
		}

		NEXT();
		return nanoc_assign(parser, assign_instr(tt), local);
	}

	THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
}

static u8r nanoc_let(NanoC_Parser *parser, u8r constant)
{
	NanoC_Token *token;
	size_t idx;
	do
	{
		NEXT();
		token = TOKEN(0);
		insert_var(parser, token, &idx);
		NEXT();
		token = TOKEN(0);
		if(token->Type == NANOC_TT_ASSIGN)
		{
			PROPAGATE(nanoc_assign(parser, 0, idx));
		}
		token = TOKEN(0);
	} while(token->Type == NANOC_TT_COMMA);
	if(token->Type != NANOC_TT_SEMICOLON)
	{
		THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
	}

	return NANOC_STATUS_SUCCESS;
}

static u8r nanoc_return(NanoC_Parser *parser)
{
	NEXT();
	if(TT(0) == NANOC_TT_SEMICOLON)
	{
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHI8, 0);
	}
	else
	{
		PROPAGATE(nanoc_expression(parser));
		EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	}

	nanoc_output_emit(&parser->Output, NANOC_INSTR_RET);
	return NANOC_STATUS_SUCCESS;
}

static u8r nanoc_parser_inc_dec(NanoC_Parser *parser, size_t *idx)
{
	NanoC_Token *token;
	NEXT();
	token = TOKEN(0);
	if(!nanoc_map_find(&parser->Variables,
		token->Ptr, token->Length, idx))
	{
		THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
	}

	NEXT();
	EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	return NANOC_STATUS_SUCCESS;
}

static u8r nanoc_dec(NanoC_Parser *parser)
{
	size_t idx;
	PROPAGATE(nanoc_parser_inc_dec(parser, &idx));
	nanoc_output_emit2(&parser->Output, NANOC_INSTR_DEC, idx);
	return NANOC_STATUS_SUCCESS;
}

static u8r nanoc_inc(NanoC_Parser *parser)
{
	size_t idx;
	PROPAGATE(nanoc_parser_inc_dec(parser, &idx));
	nanoc_output_emit2(&parser->Output, NANOC_INSTR_INC, idx);
	return NANOC_STATUS_SUCCESS;
}

u8r nanoc_statement(NanoC_Parser *parser)
{
	switch(TT(0))
	{
	case NANOC_TT_CONST:      return nanoc_let(parser, 1);
	case NANOC_TT_LET:        return nanoc_let(parser, 0);
	case NANOC_TT_IDENTIFIER: return nanoc_identifier(parser);
	case NANOC_TT_IF:         return nanoc_if(parser);
	case NANOC_TT_WHILE:      return nanoc_while(parser);
	case NANOC_TT_DO:         return nanoc_do_while(parser);
	case NANOC_TT_LOOP:       return nanoc_loop(parser);
	case NANOC_TT_BREAK:      return nanoc_break(parser);
	case NANOC_TT_CONTINUE:   return nanoc_continue(parser);
	case NANOC_TT_RETURN:     return nanoc_return(parser);
	case NANOC_TT_L_BRACE:    return nanoc_block_inner(parser);
	case NANOC_TT_DECREMENT:  return nanoc_dec(parser);
	case NANOC_TT_INCREMENT:  return nanoc_inc(parser);
	}

	return NANOC_ERROR_UNEXPECTED_TOKEN;
}