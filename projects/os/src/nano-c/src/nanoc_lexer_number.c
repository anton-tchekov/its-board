#include "nanoc_lexer_number.h"
#include "nanoc_util.h"
#include <ctype.h>

static NanoC_Bool is_binary(u8r c)
{
	return c == '0' || c == '1';
}

static NanoC_Bool is_octal(u8r c)
{
	return c >= '0' && c <= '7';
}

static u32 lexer_binary(NanoC_Lexer *lexer)
{
	u32 n = 0;
	u8r c;
	nanoc_lexer_advance(lexer);
	c = nanoc_lexer_advance(lexer);
	while(is_binary(c))
	{
		n = 2 * n + (c - '0');
		c = nanoc_lexer_advance(lexer);
	}

	return n;
}

static u32 lexer_hex(NanoC_Lexer *lexer)
{
	u32 n = 0;
	u8r c;
	nanoc_lexer_advance(lexer);
	c = nanoc_lexer_advance(lexer);
	while(isxdigit(c))
	{
		n = 16 * n + nanoc_hex_digit_value(c);
		c = nanoc_lexer_advance(lexer);
	}

	return n;
}

static u32 lexer_octal(NanoC_Lexer *lexer)
{
	u32 n = 0;
	u8r c = nanoc_lexer_advance(lexer);
	while(is_octal(c))
	{
		n = 8 * n + (c - '0');
		c = nanoc_lexer_advance(lexer);
	}

	return n;
}

static i32 lexer_decimal(NanoC_Lexer *lexer)
{
	i32 n = 0;
	u8r c = lexer->Current;
	while(isdigit(c))
	{
		n = 10 * n + (c - '0');
		c = nanoc_lexer_advance(lexer);
	}

	return n;
}

u8r nanoc_lexer_number(NanoC_Lexer *lexer, NanoC_Token *token)
{
	i32 n;
	u8r c = lexer->Current;
	if(!isdigit(c))
	{
		return 0;
	}

	token->Type = NANOC_TT_INTEGER;
	if(c == '0')
	{
		c = lexer->Ptr[1];
		if(c == 'x' && isxdigit(lexer->Ptr[2]))
		{
			n = lexer_hex(lexer);
		}
		else if(c == 'b' && is_binary(lexer->Ptr[2]))
		{
			n = lexer_binary(lexer);
		}
		else
		{
			n = lexer_octal(lexer);
		}
	}
	else
	{
		n = lexer_decimal(lexer);
	}

	token->Value.Integer = n;
	token->Length = lexer->Ptr - token->Ptr;
	return 1;
}
