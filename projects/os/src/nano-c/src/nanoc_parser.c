/**
 * @file    nanoc_parser.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_parser.h"
#include "nanoc_loop.h"
#include "nanoc_if.h"
#include "nanoc_instruction.h"
#include "nanoc_status.h"
#include "nanoc_expression.h"

static NanoC_Status insert_token(
	NanoC_Map *map, NanoC_Token *token, size_t *idx)
{
	if(token->Type != NANOC_TT_IDENTIFIER)
	{
		NANOC_THROW(NANOC_ERROR_EXPECTED_IDENTIFIER);
	}

	return nanoc_map_insert(map, token->Ptr, idx);
}

void nanoc_parser_init(NanoC_Parser *parser, const char *source,
	char *strings, u8 *output, size_t output_size,
	const NanoC_ParserBuiltins *builtins)
{
	nanoc_tokenstream_init(&parser->TokenStream, source, strings);
	nanoc_output_init(&parser->Output, output, output_size);
	nanoc_map_init(&parser->Variables, parser->VariableBuffer,
		NANOC_VARIABLE_CAPACITY);
	nanoc_map_init(&parser->Functions, parser->FunctionBuffer,
		NANOC_FUNCTION_CAPACITY);
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
	size_t var_cnt = parser->Variables.Count;
	NANOC_NEXT();
	while(NANOC_TT(0) != NANOC_TT_R_BRACE)
	{
		NANOC_PROPAGATE(nanoc_statement(parser));
		NANOC_NEXT();
	}

	parser->Variables.Count = var_cnt;
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_stmt_list(NanoC_Parser *parser)
{
	while(NANOC_TT(0) != NANOC_TT_NULL)
	{
		NANOC_PROPAGATE(nanoc_statement(parser));
		NANOC_NEXT();
	}

	nanoc_output_emit(&parser->Output, NANOC_INSTR_HALT);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_block(NanoC_Parser *parser)
{
	NANOC_EXPECT(NANOC_TT_L_BRACE, NANOC_ERROR_EXPECTED_L_BRACE);
	return nanoc_block_inner(parser);
}

static NanoC_Bool check_fn_args(size_t cnt, size_t expected,
	NanoC_Bool is_variadic)
{
	return is_variadic ? (cnt >= expected) : (cnt == expected);
}

NanoC_Status nanoc_fn_call(NanoC_Parser *parser)
{
	NanoC_Token *token;
	size_t fn_id, fn_args, arg_cnt;
	NanoC_Bool is_variadic;
	NanoC_Address fn_addr;

	arg_cnt = 0;
	is_variadic = 0;
	token = NANOC_TOKEN(0);
	if((fn_id = nanoc_builtin_find(parser->Builtins, token->Ptr)))
	{
		const NanoC_ParserBuiltin *builtin;
		--fn_id;
		builtin = &parser->Builtins->Table[fn_id];
		fn_addr = -fn_id - 1;
		fn_args = builtin->NumArgs;
		is_variadic = builtin->IsVariadic;
	}
	else if((fn_id = nanoc_map_find(&parser->Functions, token->Ptr)))
	{
		--fn_id;
		fn_addr = parser->FunctionAddrs[fn_id];
		fn_args = parser->FunctionArgs[fn_id];
	}
	else
	{
		NANOC_THROW(NANOC_ERROR_UNDEFINED_FN);
	}

	NANOC_NEXT();
	NANOC_NEXT();
	if(NANOC_TT(0) != NANOC_TT_R_PAREN)
	{
		for(;;)
		{
			NanoC_TokenType tt;
			NANOC_PROPAGATE(nanoc_expression(parser));
			++arg_cnt;
			tt = NANOC_TT(0);
			if(tt == NANOC_TT_R_PAREN)
			{
				break;
			}
			else if(tt == NANOC_TT_COMMA)
			{
				NANOC_NEXT();
			}
			else
			{
				NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
			}
		}
	}

	if(!check_fn_args(arg_cnt, fn_args, is_variadic))
	{
		NANOC_THROW(NANOC_ERROR_FN_NUM_ARGS);
	}

	nanoc_output_emit2(&parser->Output, NANOC_INSTR_CALL, arg_cnt);
	nanoc_output_emit16(&parser->Output, fn_addr);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_assign(
	NanoC_Parser *parser, NanoC_Opcode op, size_t idx)
{
	NANOC_NEXT();
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
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_identifier(NanoC_Parser *parser)
{
	NanoC_Token *token = NANOC_TOKEN(0);
	NanoC_TokenType tt = NANOC_TT(1);
	if(tt == NANOC_TT_L_PAREN)
	{
		return nanoc_action(parser);
	}
	else if(is_assign(tt))
	{
		size_t local;
		if(!(local = nanoc_map_find(&parser->Variables, token->Ptr)))
		{
			NANOC_THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
		}

		--local;
		NANOC_NEXT();
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
		NANOC_NEXT();
		token = NANOC_TOKEN(0);
		NANOC_PROPAGATE(insert_token(&parser->Variables, token, &idx));
		++parser->NumLocals;
		NANOC_NEXT();
		token = NANOC_TOKEN(0);
		if(token->Type == NANOC_TT_ASSIGN)
		{
			NANOC_PROPAGATE(nanoc_assign(parser, 0, idx));
		}
		token = NANOC_TOKEN(0);
	} while(token->Type == NANOC_TT_COMMA);
	if(token->Type != NANOC_TT_SEMICOLON)
	{
		NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
	}

	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_return(NanoC_Parser *parser)
{
	parser->ReturnFlag = 1;
	NANOC_NEXT();
	if(NANOC_TT(0) == NANOC_TT_SEMICOLON)
	{
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHI8, 0);
	}
	else
	{
		NANOC_PROPAGATE(nanoc_expression(parser));
		NANOC_EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	}

	nanoc_output_emit(&parser->Output, NANOC_INSTR_RET);
	return NANOC_STATUS_SUCCESS;
}

static NanoC_Status nanoc_parser_inc_dec(
	NanoC_Parser *parser, NanoC_Opcode instr)
{
	size_t idx;
	NanoC_Token *token;
	NANOC_NEXT();
	token = NANOC_TOKEN(0);
	if(!(idx = nanoc_map_find(&parser->Variables, token->Ptr)))
	{
		NANOC_THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
	}

	--idx;
	NANOC_NEXT();
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
		tt = NANOC_TT(0);
		switch(tt)
		{
		case NANOC_TT_IDENTIFIER: NANOC_PROPAGATE(nanoc_identifier(parser)); break;
		case NANOC_TT_DECREMENT:  NANOC_PROPAGATE(nanoc_dec(parser));        break;
		case NANOC_TT_INCREMENT:  NANOC_PROPAGATE(nanoc_inc(parser));        break;
		default: NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
		}

		tt = NANOC_TT(0);
		if(tt == end)
		{
			return NANOC_STATUS_SUCCESS;
		}

		NANOC_NEXT();
	}
	while(tt == NANOC_TT_COMMA);
	NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
}

NanoC_Status nanoc_statement(NanoC_Parser *parser)
{
	switch(NANOC_TT(0))
	{
	case NANOC_TT_IF:        return nanoc_if(parser);
	case NANOC_TT_WHILE:     return nanoc_while(parser);
	case NANOC_TT_DO:        return nanoc_do_while(parser);
	case NANOC_TT_LOOP:      return nanoc_loop(parser);
	case NANOC_TT_FOR:       return nanoc_for(parser);
	case NANOC_TT_BREAK:     return nanoc_break(parser);
	case NANOC_TT_CONTINUE:  return nanoc_continue(parser);
	case NANOC_TT_RETURN:    return nanoc_return(parser);
	case NANOC_TT_L_BRACE:   return nanoc_block_inner(parser);
	case NANOC_TT_INT:       return nanoc_int(parser);
	default:
		return nanoc_substmt(parser, NANOC_TT_SEMICOLON);
	}

	return NANOC_ERROR_UNEXPECTED_TOKEN;
}

NanoC_Status nanoc_function(NanoC_Parser *parser)
{
	size_t vidx, fidx, params;
	NanoC_Address addr;
	NanoC_Token *token;

	parser->ReturnFlag = 0;
	parser->NumLocals = 0;
	addr = nanoc_output_isp(&parser->Output);

	NANOC_EXPECT(NANOC_TT_INT, NANOC_ERROR_EXPECTED_FN);
	NANOC_NEXT();
	token = NANOC_TOKEN(0);

	if(nanoc_builtin_find(parser->Builtins, token->Ptr))
	{
		NANOC_THROW(NANOC_ERROR_REDEFINITION);
	}

	NANOC_PROPAGATE(insert_token(&parser->Functions, token, &fidx));
	parser->FunctionAddrs[fidx] = addr;

	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	params = 0;
	NANOC_NEXT();
	if(NANOC_TT(0) != NANOC_TT_R_PAREN)
	{
		for(;;)
		{
			NanoC_TokenType tt;
			token = NANOC_TOKEN(0);
			NANOC_PROPAGATE(insert_token(&parser->Variables, token, &vidx));
			++params;
			++parser->NumLocals;
			NANOC_NEXT();
			tt = NANOC_TT(0);
			if(tt == NANOC_TT_R_PAREN)
			{
				break;
			}
			else if(tt == NANOC_TT_COMMA)
			{
				NANOC_NEXT();
			}
			else
			{
				NANOC_THROW(NANOC_ERROR_UNEXPECTED_TOKEN);
			}
		}
	}

	parser->FunctionArgs[fidx] = params;
	NANOC_NEXT();
	NANOC_PROPAGATE(nanoc_block(parser));
	nanoc_output_isp_amount(&parser->Output, addr, parser->NumLocals);
	parser->Variables.Count = 0;

	if(!parser->ReturnFlag)
	{
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHI8, 0);
		nanoc_output_emit(&parser->Output, NANOC_INSTR_RET);
	}

	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_file(NanoC_Parser *parser)
{
	size_t fn_id;

	nanoc_output_emit2(&parser->Output, NANOC_INSTR_CALL, 0);
	parser->Output.Pos += 2;
	nanoc_output_emit(&parser->Output, NANOC_INSTR_HALT);

	while(NANOC_TT(0) != NANOC_TT_NULL)
	{
		NANOC_PROPAGATE(nanoc_function(parser));
		NANOC_NEXT();
	}

	if(!(fn_id = nanoc_map_find(&parser->Functions, "main")))
	{
		NANOC_THROW(NANOC_ERROR_NO_MAIN);
	}

	--fn_id;
	nanoc_output_emit_addr_at(&parser->Output, 2, parser->FunctionAddrs[fn_id]);
	return NANOC_STATUS_SUCCESS;
}
