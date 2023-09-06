#include "nanoc_lexer_identifier.h"
#include "nanoc_util.h"
#include <ctype.h>

static NanoC_Bool is_identifer_start(u8r c)
{
	return isalpha(c) || c == '_';
}

static NanoC_Bool is_identifier_char(u8r c)
{
	return isalnum(c) || c == '_';
}

static NanoC_TokenType keyword_detect(const char *ident, size_t len)
{
	static const u8 keywords[] =
	{
		'i', 'f', NANOC_LUT_ST(NANOC_TT_IF),
		'e', 'l', 's', 'e', NANOC_LUT_ST(NANOC_TT_ELSE),
		'd', 'o', NANOC_LUT_ST(NANOC_TT_DO),
		'w', 'h', 'i', 'l', 'e', NANOC_LUT_ST(NANOC_TT_WHILE),
		'f', 'o', 'r', NANOC_LUT_ST(NANOC_TT_FOR),
		'l', 'o', 'o', 'p', NANOC_LUT_ST(NANOC_TT_LOOP),
		'l', 'e', 't', NANOC_LUT_ST(NANOC_TT_LET),
		'f', 'n', NANOC_LUT_ST(NANOC_TT_FN),
		'b', 'r', 'e', 'a', 'k', NANOC_LUT_ST(NANOC_TT_BREAK),
		'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', NANOC_LUT_ST(NANOC_TT_CONTINUE),
		'r', 'e', 't', 'u', 'r', 'n', NANOC_LUT_ST(NANOC_TT_RETURN),
		'D', 'C', 'B', NANOC_LUT_ST(NANOC_TT_DCB),
		'D', 'C', 'W', NANOC_LUT_ST(NANOC_TT_DCW),
		'D', 'C', 'D', NANOC_LUT_ST(NANOC_TT_DCD),
		'F', 'I', 'L', 'L', NANOC_LUT_ST(NANOC_TT_FILL),
		NANOC_LUT_ST(NANOC_TT_IDENTIFIER)
	};

	const u8 *r = keywords;
	for(;;)
	{
		const char *s = ident;
		while(*s == *r) { ++s; ++r; }
		if(NANOC_LUT_FOUND(*r) && ((s == (ident + len)) || s == ident))
		{
			return NANOC_LUT_LD(*r);
		}

		while(!NANOC_LUT_FOUND(*r)) { ++r; }
		++r;
	}
}

NanoC_Status nanoc_lexer_identifier(NanoC_Lexer *lexer, NanoC_Token *token)
{
	NanoC_Char c = lexer->Current;
	if(!is_identifer_start(c))
	{
		return 0;
	}

	do
	{
		c = nanoc_lexer_advance(lexer);
	}
	while(is_identifier_char(c));
	token->Length = lexer->Ptr - token->Ptr;
	token->Type = keyword_detect(token->Ptr, token->Length);
	return 1;
}
