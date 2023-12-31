/**
 * @file    nanoc_expression.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_expression.h"
#include "nanoc_debug.h"
#include "nanoc_instruction.h"
#include "nanoc_status.h"
#include "nanoc_util.h"

typedef u8r NanoC_TokenClass;
typedef u8r NanoC_Precedence;

enum
{
	TC_OPERAND,
	TC_BINARY,
	TC_UNARY,
	TC_COUNT
};

#define LUT(X) ((X) - NANOC_TT_OP_START)

#ifdef NANOC_DEBUG

static const char *token_class_tostring(NanoC_TokenClass token_class)
{
	static const char *lut[] =
	{
		[TC_OPERAND] = "TC_OPERAND",
		[TC_BINARY]  = "TC_BINARY",
		[TC_UNARY]   = "TC_UNARY"
	};

	nanoc_assert(token_class < TC_COUNT);
	return lut[token_class];
}

#ifdef NANOC_DESKTOP

static void token_class_print(NanoC_TokenClass token_class)
{
	printf("TOKEN CLASS = %s\n", token_class_tostring(token_class));
}

#endif /* NANOC_DESKTOP */
#endif /* NANOC_DEBUG */

static NanoC_Bool token_is_operator(NanoC_TokenType token_type)
{
	return (token_type >= NANOC_TT_OP_START) &&
		(token_type <= NANOC_TT_OP_END);
}

static NanoC_Precedence precedence_get(NanoC_TokenType token_type)
{
	static const u8 precedence_lut[] =
	{
		[ LUT(NANOC_TT_L_OR) ]    = 11,
		[ LUT(NANOC_TT_L_AND) ]   = 10,
		[ LUT(NANOC_TT_B_OR) ]    =  9,
		[ LUT(NANOC_TT_B_XOR) ]   =  8,
		[ LUT(NANOC_TT_B_AND) ]   =  7,
		[ LUT(NANOC_TT_EQ) ]      =  6,
		[ LUT(NANOC_TT_NE) ]      =  6,
		[ LUT(NANOC_TT_LT) ]      =  5,
		[ LUT(NANOC_TT_GT) ]      =  5,
		[ LUT(NANOC_TT_LE) ]      =  5,
		[ LUT(NANOC_TT_GE) ]      =  5,
		[ LUT(NANOC_TT_SHL) ]     =  4,
		[ LUT(NANOC_TT_SHR) ]     =  4,
		[ LUT(NANOC_TT_ADD) ]     =  3,
		[ LUT(NANOC_TT_SUB) ]     =  3,
		[ LUT(NANOC_TT_MUL) ]     =  2,
		[ LUT(NANOC_TT_DIV) ]     =  2,
		[ LUT(NANOC_TT_MOD) ]     =  2,
		[ LUT(NANOC_TT_L_NOT) ]   =  1,
		[ LUT(NANOC_TT_B_NOT) ]   =  1,
		[ LUT(NANOC_TT_U_MINUS) ] =  1,
	};

	nanoc_assert(token_is_operator(token_type));
	return precedence_lut[LUT(token_type)];
}

static NanoC_Opcode token_to_instr(NanoC_TokenType token_type)
{
	nanoc_assert(token_is_operator(token_type));
	return token_type - NANOC_TT_INSTR_OP_START + NANOC_INSTR_OP_START;
}

static void nanoc_expression_number(NanoC_Parser *parser, NanoC_Token *token)
{
	nanoc_output_pushi(&parser->Output, token->Value);
}

static NanoC_Status nanoc_expression_identifier(NanoC_Parser *parser)
{
	size_t local;
	NanoC_Token *token = NANOC_TOKEN(0);
	if(NANOC_TT(1) == NANOC_TT_L_PAREN)
	{
		NANOC_PROPAGATE(nanoc_fn_call(parser));
	}
	else
	{
		if(!(local = nanoc_map_find(&parser->Variables, token->Ptr)))
		{
			NANOC_THROW(NANOC_ERROR_UNDEFINED_VARIABLE);
		}

		--local;
		nanoc_output_emit2(&parser->Output, NANOC_INSTR_PUSHL, local);
	}

	return NANOC_STATUS_SUCCESS;
}

static void and_or_addr_here(NanoC_Parser *parser, size_t prev_top)
{
	size_t i = parser->AndOrTop;
	while(i > prev_top)
	{
		--i;
		nanoc_output_jump_here(&parser->Output, parser->AndOrStack[i]);
	}

	parser->AndOrTop = prev_top;
}

static void nanoc_expression_operator(NanoC_Parser *parser,
	size_t local_top, size_t ao_top, NanoC_TokenType tt)
{
	NanoC_Precedence prec = precedence_get(tt);

	while((parser->OpTop > local_top) &&
		(precedence_get(parser->OperatorStack[parser->OpTop - 1]) <= prec))
	{
		--parser->OpTop;
		nanoc_output_emit(&parser->Output,
			token_to_instr(parser->OperatorStack[parser->OpTop]));
	}

	if(tt == NANOC_TT_L_AND)
	{
		parser->AndOrStack[parser->AndOrTop++] =
			nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_PJZ);
	}
	else if(tt == NANOC_TT_L_OR)
	{
		and_or_addr_here(parser, ao_top);

		parser->AndOrStack[parser->AndOrTop++] =
			nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_PJNZ);
	}
	else
	{
		parser->OperatorStack[parser->OpTop++] = tt;
	}
}

static NanoC_Status check_prev(
	NanoC_TokenClass prev_class, NanoC_TokenClass cur_class)
{
	static const u8 lut[TC_COUNT][TC_COUNT] =
	{
		[TC_OPERAND][TC_OPERAND] = NANOC_ERROR_UNEXPECTED_TOKEN,
		[TC_OPERAND][TC_BINARY]  = NANOC_STATUS_SUCCESS,
		[TC_OPERAND][TC_UNARY]   = NANOC_ERROR_UNEXPECTED_TOKEN,

		[TC_BINARY][TC_OPERAND]  = NANOC_STATUS_SUCCESS,
		[TC_BINARY][TC_BINARY]   = NANOC_ERROR_UNEXPECTED_TOKEN,
		[TC_BINARY][TC_UNARY]    = NANOC_STATUS_SUCCESS,

		[TC_UNARY][TC_OPERAND]   = NANOC_STATUS_SUCCESS,
		[TC_UNARY][TC_BINARY]    = NANOC_ERROR_UNEXPECTED_TOKEN,
		[TC_UNARY][TC_UNARY]     = NANOC_ERROR_UNEXPECTED_TOKEN
	};

	return lut[prev_class][cur_class];
}

static NanoC_TokenClass token_class(NanoC_TokenType tt)
{
	NanoC_TokenClass token_class = TC_BINARY;
	if(tt == NANOC_TT_INTEGER ||
		tt == NANOC_TT_IDENTIFIER ||
		tt == NANOC_TT_L_PAREN)
	{
		token_class = TC_OPERAND;
	}
	else if(tt == NANOC_TT_U_MINUS ||
		tt == NANOC_TT_L_NOT ||
		tt == NANOC_TT_B_NOT)
	{
		token_class = TC_UNARY;
	}

	return token_class;
}

NanoC_Status nanoc_expression(NanoC_Parser *parser)
{
	NanoC_Token *cur_token;
	size_t local_top = parser->OpTop;
	size_t aoprev_top = parser->AndOrTop;
	NanoC_TokenType tt = NANOC_TT_NULL;
	NanoC_TokenClass prev_class;

	for(;;)
	{
		prev_class = token_class(tt);
		cur_token = nanoc_tokenstream_get(&parser->TokenStream, 0);
		tt = cur_token->Type;
		if((tt == NANOC_TT_SUB) && (prev_class == TC_BINARY))
		{
			tt = NANOC_TT_U_MINUS;
		}

		NANOC_PROPAGATE(check_prev(prev_class, token_class(tt)));
		if(tt == NANOC_TT_INTEGER)
		{
			nanoc_expression_number(parser, cur_token);
		}
		else if(tt == NANOC_TT_IDENTIFIER)
		{
			NANOC_PROPAGATE(nanoc_expression_identifier(parser));
		}
		else if(tt == NANOC_TT_L_PAREN)
		{
			NANOC_NEXT();
			NANOC_PROPAGATE(nanoc_expression(parser));
			NANOC_EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
		}
		else if(token_is_operator(tt))
		{
			nanoc_expression_operator(parser, local_top, aoprev_top, tt);
		}
		else
		{
			break;
		}

		NANOC_NEXT();
	}

	while(parser->OpTop > local_top)
	{
		--parser->OpTop;
		nanoc_output_emit(&parser->Output,
			token_to_instr(parser->OperatorStack[parser->OpTop]));
	}

	and_or_addr_here(parser, aoprev_top);
	return NANOC_STATUS_SUCCESS;
}
