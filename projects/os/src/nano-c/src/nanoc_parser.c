#include "nanoc_parser.h"
#include "nanoc_loop.h"
#include "nanoc_if.h"
#include "nanoc_instruction.h"
#include "nanoc_status.h"
#include "nanoc_expression.h"

static NanoC_Status insert_var(
	NanoC_Parser *parser, NanoC_Token *token, size_t *idx)
{
	NanoC_Status ret;
	if(token->Type != NANOC_TT_IDENTIFIER)
	{
		NANOC_THROW(NANOC_ERROR_EXPECTED_IDENTIFIER);
	}

	ret = nanoc_map_insert(&parser->Variables,
		token->Ptr, token->Length, idx);

	if(ret == NANOC_ERROR_STACK_OVERFLOW)
	{
		NANOC_THROW(NANOC_ERROR_TOO_MANY_VARIABLES);
	}
	else if(ret == NANOC_ERROR_DUPLICATE_MAP_ELEMENT)
	{
		NANOC_THROW(NANOC_ERROR_VARIABLE_REDEFINITION);
	}

	return NANOC_STATUS_SUCCESS;
}

void nanoc_parser_init(NanoC_Parser *parser, const char *source,
	char *strings, u8 *output, size_t output_size,
	const NanoC_ParserBuiltins *builtins)
{
	nanoc_tokenstream_init(&parser->TokenStream, source, strings);
	nanoc_output_init(&parser->Output, output, output_size);
	nanoc_map_init(&parser->Variables, parser->VariableBuffer,
		NANOC_VARIABLE_CAPACITY);
	parser->Builtins = builtins;
	parser->OpTop = 0;
	parser->AndOrTop = 0;
	parser->BreakNesting = 0;
	parser->ContinueNesting = 0;
	nanoc_address_stack_init(&parser->BreakStack, parser->BreakBuffer);
	nanoc_address_stack_init(&parser->ContinueStack, parser->ContinueBuffer);
}

NanoC_Status nanoc_block_inner(NanoC_Parser *parser)
{
	size_t var_cnt = nanoc_map_count(&parser->Variables);
	NEXT();
	while(TT(0) != NANOC_TT_R_BRACE)
	{
		NANOC_PROPAGATE(nanoc_statement(parser));
		NEXT();
	}

	nanoc_map_reset(&parser->Variables, var_cnt);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_block(NanoC_Parser *parser)
{
	EXPECT(NANOC_TT_L_BRACE, NANOC_ERROR_EXPECTED_L_BRACE);
	return nanoc_block_inner(parser);
}

static NanoC_Bool check_fn_args(u8r cnt, u8r expected,
	NanoC_Bool is_variadic)
{
	return is_variadic ? (cnt >= expected) : (cnt == expected);
}

NanoC_Status nanoc_fn_call(NanoC_Parser *parser)
{
	u8r arg_cnt = 0;
	NanoC_Token *token = TOKEN(0);
	const NanoC_ParserBuiltin *builtin;
	int builtin_id = nanoc_builtin_find(parser->Builtins,
		token->Ptr, token->Length);

	if(builtin_id < 0)
	{
		NANOC_THROW(NANOC_ERROR_UNDEFINED_FN);
	}

	builtin = &parser->Builtins->Table[builtin_id];

	NEXT();
	NEXT();
	if(TT(0) != NANOC_TT_R_PAREN)
	{
		for(;;)
		{
			NanoC_TokenType tt;
			NANOC_PROPAGATE(nanoc_expression(parser));
			++arg_cnt;
			if(arg_cnt >= NANOC_MAX_FN_ARGS)
			{
				NANOC_THROW(NANOC_ERROR_TOO_MANY_FN_ARGS);
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
				NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
			}
		}
	}

	if(!check_fn_args(arg_cnt, builtin->NumArgs, builtin->IsVariadic))
	{
		NANOC_THROW(NANOC_ERROR_FN_NUM_ARGS);
	}

	nanoc_output_emit2(&parser->Output, NANOC_INSTR_CALL, arg_cnt);
	nanoc_output_emit16(&parser->Output, -builtin_id - 1);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_assign(
	NanoC_Parser *parser, NanoC_Opcode op, size_t idx)
{
	NEXT();
	if(op)
	{
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHL, idx);
	}

	NANOC_PROPAGATE(nanoc_expression(parser));
	if(op)
	{
		nanoc_output_emit(&parser->Output, op);
	}

	nanoc_output_emit2(&parser->Output, NANOC_INSTR_POPL, idx);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Bool is_assign(NanoC_TokenType tt)
{
	return tt >= NANOC_TT_ASSIGN_START && tt <= NANOC_TT_ASSIGN_END;
}

#define ASSIGN_LUT(X) ((X) - NANOC_TT_ASSIGN_START)

static NanoC_Opcode assign_instr(NanoC_TokenType tt)
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

static NanoC_Status nanoc_action(NanoC_Parser *parser)
{
	NANOC_PROPAGATE(nanoc_fn_call(parser));
	nanoc_output_emit(&parser->Output, NANOC_INSTR_POP);
	NEXT();
	EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_identifier(NanoC_Parser *parser)
{
	NanoC_Token *token = TOKEN(0);
	NanoC_TokenType tt = TT(1);
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
			NANOC_THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
		}

		NEXT();
		return nanoc_assign(parser, assign_instr(tt), local);
	}

	NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
}

NanoC_Status nanoc_int(NanoC_Parser *parser)
{
	NanoC_Token *token;
	size_t idx;
	do
	{
		NEXT();
		token = TOKEN(0);
		NANOC_PROPAGATE(insert_var(parser, token, &idx));
		NEXT();
		token = TOKEN(0);
		if(token->Type == NANOC_TT_ASSIGN)
		{
			NANOC_PROPAGATE(nanoc_assign(parser, 0, idx));
		}
		token = TOKEN(0);
	} while(token->Type == NANOC_TT_COMMA);
	if(token->Type != NANOC_TT_SEMICOLON)
	{
		NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
	}

	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_return(NanoC_Parser *parser)
{
	NEXT();
	if(TT(0) == NANOC_TT_SEMICOLON)
	{
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHI8, 0);
	}
	else
	{
		NANOC_PROPAGATE(nanoc_expression(parser));
		EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	}

	nanoc_output_emit(&parser->Output, NANOC_INSTR_RET);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_parser_inc_dec(
	NanoC_Parser *parser, NanoC_Opcode instr)
{
	size_t idx;
	NanoC_Token *token;
	NEXT();
	token = TOKEN(0);
	if(!nanoc_map_find(&parser->Variables,
		token->Ptr, token->Length, &idx))
	{
		NANOC_THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
	}

	NEXT();
	nanoc_output_emit2(&parser->Output, instr, idx);
	return NANOC_STATUS_SUCCESS;
}

static inline NanoC_Status nanoc_dec(NanoC_Parser *parser)
{
	return nanoc_parser_inc_dec(parser, NANOC_INSTR_DEC);
}

static inline NanoC_Status nanoc_inc(NanoC_Parser *parser)
{
	return nanoc_parser_inc_dec(parser, NANOC_INSTR_INC);
}

NanoC_Status nanoc_substmt(NanoC_Parser *parser, NanoC_TokenType end)
{
	NanoC_TokenType tt;
	do
	{
		tt = TT(0);
		switch(tt)
		{
		case NANOC_TT_IDENTIFIER: NANOC_PROPAGATE(nanoc_identifier(parser)); break;
		case NANOC_TT_DECREMENT:  NANOC_PROPAGATE(nanoc_dec(parser));        break;
		case NANOC_TT_INCREMENT:  NANOC_PROPAGATE(nanoc_inc(parser));        break;
		default: NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
		}

		tt = TT(0);
		if(tt == end)
		{
			return NANOC_STATUS_SUCCESS;
		}

		NEXT();
	}
	while(tt == NANOC_TT_COMMA);
	NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
}

NanoC_Status nanoc_statement(NanoC_Parser *parser)
{
	switch(TT(0))
	{
	case NANOC_TT_IF:         return nanoc_if(parser);
	case NANOC_TT_WHILE:      return nanoc_while(parser);
	case NANOC_TT_DO:         return nanoc_do_while(parser);
	case NANOC_TT_LOOP:       return nanoc_loop(parser);
	case NANOC_TT_FOR:        return nanoc_for(parser);
	case NANOC_TT_BREAK:      return nanoc_break(parser);
	case NANOC_TT_CONTINUE:   return nanoc_continue(parser);
	case NANOC_TT_RETURN:     return nanoc_return(parser);
	case NANOC_TT_L_BRACE:    return nanoc_block_inner(parser);
	case NANOC_TT_INT:        return nanoc_int(parser);
	default:
		return nanoc_substmt(parser, NANOC_TT_SEMICOLON);
	}

	return NANOC_ERROR_UNEXPECTED_TOKEN;
}
