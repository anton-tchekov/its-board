#include "nanoc_lexer_literal.h"
#include "nanoc_util.h"
#include <ctype.h>

static i32 _escape_sequence(const char *p, size_t *out_len)
{
	static const u8 tab[] =
	{
		'\\', NANOC_LUT_ST('\\'),
		'\'', NANOC_LUT_ST('\''),
		'\"', NANOC_LUT_ST('\"'),
		't',  NANOC_LUT_ST('\t'),
		'v',  NANOC_LUT_ST('\v'),
		'r',  NANOC_LUT_ST('\r'),
		'n',  NANOC_LUT_ST('\n'),
		'b',  NANOC_LUT_ST('\b'),
		'0',  NANOC_LUT_ST('\0'),
		NANOC_LUT_ST('\0')
	};

	const u8 *t;
	u8r c = *p;
	if(c == 'x' && isxdigit(p[1]) && isxdigit(p[2]))
	{
		*out_len = 3;
		return 16 * nanoc_hex_digit_value(p[1]) + nanoc_hex_digit_value(p[2]);
	}

	for(t = tab; *t; t += 2)
	{
		if(c == *t)
		{
			*out_len = 1;
			return NANOC_LUT_LD(t[1]);
		}
	}

	return -1;
}

u8r nanoc_lexer_char(NanoC_Lexer *lexer, NanoC_Token *token)
{
	const char *p;
	u8r c;
	i32 v;

	p = lexer->Ptr;
	if(*p != '\'')
	{
		return 0;
	}

	++p;
	c = *p;
	if(c == '\\')
	{
		size_t len;
		++p;
		v = _escape_sequence(p, &len);
		if(v < 0)
		{
			return 0;
		}

		p += len;
	}
	else if(isprint(c) && c != '\'')
	{
		v = c;
		++p;
	}
	else
	{
		return 0;
	}

	if(*p != '\'')
	{
		return 0;
	}

	++p;
	nanoc_lexer_forward(lexer, p);
	token->Value.Integer = v;
	token->Type = NANOC_TT_INTEGER;
	token->Length = p - token->Ptr;
	return 1;
}

u8r nanoc_lexer_string(NanoC_Lexer *lexer, NanoC_Token *token)
{
	const char *p;
	int c, v;

	p = lexer->Ptr;
	if(*p != '\"')
	{
		return 0;
	}

	++p;
	while((c = *p) != '\"')
	{
		if(c == '\\')
		{
			size_t len;
			++p;
			v = _escape_sequence(p, &len);
			if(v < 0)
			{
				return 0;
			}

			p += len;
		}
		else if(isprint(c))
		{
			v = c;
			++p;
		}
		else
		{
			return 0;
		}
	}

	++p;
	nanoc_lexer_forward(lexer, p);
	token->Type = NANOC_TT_STRING;
	token->Length = p - token->Ptr;
	return 1;
}
