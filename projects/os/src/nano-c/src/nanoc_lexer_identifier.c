/**
 * @file    nanoc_lexer_identifier.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_lexer_identifier.h"
#include "nanoc_util.h"
#include "ctype_ext.h"

NanoC_TokenType nanoc_keyword_detect(const char *ident, size_t len)
{
	static const u8 keywords[] =
	{
		'f', 'n', NANOC_LUT_ST(NANOC_TT_FN),
		'i', 'f', NANOC_LUT_ST(NANOC_TT_IF),
		'e', 'l', 's', 'e', NANOC_LUT_ST(NANOC_TT_ELSE),
		'd', 'o', NANOC_LUT_ST(NANOC_TT_DO),
		'w', 'h', 'i', 'l', 'e', NANOC_LUT_ST(NANOC_TT_WHILE),
		'f', 'o', 'r', NANOC_LUT_ST(NANOC_TT_FOR),
		'l', 'o', 'o', 'p', NANOC_LUT_ST(NANOC_TT_LOOP),
		'i', 'n', 't', NANOC_LUT_ST(NANOC_TT_INT),
		'b', 'r', 'e', 'a', 'k', NANOC_LUT_ST(NANOC_TT_BREAK),
		'c', 'o', 'n', 't', 'i', 'n', 'u', 'e', NANOC_LUT_ST(NANOC_TT_CONTINUE),
		'r', 'e', 't', 'u', 'r', 'n', NANOC_LUT_ST(NANOC_TT_RETURN),
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
	token->Type = nanoc_keyword_detect(token->Ptr, token->Length);
	return 1;
}
