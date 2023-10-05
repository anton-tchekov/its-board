#ifndef __NANOC_LEXER_H__
#define __NANOC_LEXER_H__

#include "nanoc_types.h"
#include "nanoc_token.h"

typedef struct NANOC_LEXER
{
	NanoC_Position Pos;
	const char *Ptr;
	const char *LineBegin;
	char *Strings;
	u8 Current;
} NanoC_Lexer;

static inline u8r nanoc_hex_digit_value(NanoC_Char c)
{
	return (c & 15) + (c >= 'A' ? 9 : 0);
}

void nanoc_lexer_init(NanoC_Lexer *lexer, const char *src, char *strings);
void nanoc_lexer_next(NanoC_Lexer *lexer, NanoC_Token *token);

void nanoc_lexer_forward(NanoC_Lexer *lexer, const char *p);
NanoC_Char nanoc_lexer_advance(NanoC_Lexer *lexer);

#endif /* __NANOC_LEXER_H__ */
