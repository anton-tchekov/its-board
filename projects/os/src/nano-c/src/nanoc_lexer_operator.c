#include "nanoc_lexer_operator.h"
#include "nanoc_util.h"
#include <ctype.h>

static NanoC_TokenType operator_detect(const char *p, size_t *out_len)
{
	/* Must be sorted from longer to shorter operator
		strings because of common prefixes */
	static const u8 op_list[] =
	{
		'<', '<', '=', NANOC_LUT_ST(NANOC_TT_SHL_ASSIGN),
		'>', '>', '=', NANOC_LUT_ST(NANOC_TT_SHR_ASSIGN),

		'!', '=', NANOC_LUT_ST(NANOC_TT_NE),
		'=', '=', NANOC_LUT_ST(NANOC_TT_EQ),
		'>', '=', NANOC_LUT_ST(NANOC_TT_GE),
		'<', '=', NANOC_LUT_ST(NANOC_TT_LE),
		'<', '<', NANOC_LUT_ST(NANOC_TT_SHL),
		'>', '>', NANOC_LUT_ST(NANOC_TT_SHR),
		'&', '&', NANOC_LUT_ST(NANOC_TT_L_AND),
		'|', '|', NANOC_LUT_ST(NANOC_TT_L_OR),
		'+', '=', NANOC_LUT_ST(NANOC_TT_ADD_ASSIGN),
		'-', '=', NANOC_LUT_ST(NANOC_TT_SUB_ASSIGN),
		'*', '=', NANOC_LUT_ST(NANOC_TT_MUL_ASSIGN),
		'/', '=', NANOC_LUT_ST(NANOC_TT_DIV_ASSIGN),
		'%', '=', NANOC_LUT_ST(NANOC_TT_MOD_ASSIGN),
		'~', '=', NANOC_LUT_ST(NANOC_TT_B_NOT_ASSIGN),
		'&', '=', NANOC_LUT_ST(NANOC_TT_B_AND_ASSIGN),
		'|', '=', NANOC_LUT_ST(NANOC_TT_B_OR_ASSIGN),
		'^', '=', NANOC_LUT_ST(NANOC_TT_XOR_ASSIGN),
		'+', '+', NANOC_LUT_ST(NANOC_TT_INCREMENT),
		'-', '-', NANOC_LUT_ST(NANOC_TT_DECREMENT),
		'-', '>', NANOC_LUT_ST(NANOC_TT_ARROW),

		'(', NANOC_LUT_ST(NANOC_TT_L_PAREN),
		')', NANOC_LUT_ST(NANOC_TT_R_PAREN),
		'[', NANOC_LUT_ST(NANOC_TT_L_BRACKET),
		']', NANOC_LUT_ST(NANOC_TT_R_BRACKET),
		'{', NANOC_LUT_ST(NANOC_TT_L_BRACE),
		'}', NANOC_LUT_ST(NANOC_TT_R_BRACE),
		'<', NANOC_LUT_ST(NANOC_TT_LT),
		'>', NANOC_LUT_ST(NANOC_TT_GT),
		'+', NANOC_LUT_ST(NANOC_TT_ADD),
		'-', NANOC_LUT_ST(NANOC_TT_SUB),
		'*', NANOC_LUT_ST(NANOC_TT_MUL),
		'/', NANOC_LUT_ST(NANOC_TT_DIV),
		'%', NANOC_LUT_ST(NANOC_TT_MOD),
		'!', NANOC_LUT_ST(NANOC_TT_L_NOT),
		'~', NANOC_LUT_ST(NANOC_TT_B_NOT),
		'^', NANOC_LUT_ST(NANOC_TT_B_XOR),
		'&', NANOC_LUT_ST(NANOC_TT_B_AND),
		'|', NANOC_LUT_ST(NANOC_TT_B_OR),
		':', NANOC_LUT_ST(NANOC_TT_COLON),
		';', NANOC_LUT_ST(NANOC_TT_SEMICOLON),
		'.', NANOC_LUT_ST(NANOC_TT_DOT),
		',', NANOC_LUT_ST(NANOC_TT_COMMA),
		'=', NANOC_LUT_ST(NANOC_TT_ASSIGN),
	};

	const u8 *r = op_list;
	const u8 *end = op_list + sizeof(op_list);
	while(r < end)
	{
		/* SENATUS POPULUSQUE ROMANUS */
		const char *s = p;
		const u8 *q = r;

		while(*s == *r) { ++s; ++r; }
		if(NANOC_LUT_FOUND(*r))
		{
			*out_len = r - q;
			return NANOC_LUT_LD(*r);
		}

		while(!NANOC_LUT_FOUND(*r)) { ++r; }
		++r;
	}

	*out_len = 1;
	return NANOC_TT_UNKNOWN;
}

void nanoc_lexer_operator(NanoC_Lexer *lexer, NanoC_Token *token)
{
	size_t len;
	token->Type = operator_detect(lexer->Ptr, &len);
	token->Length = len;
	nanoc_lexer_forward(lexer, lexer->Ptr + len);
}
