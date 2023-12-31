/**
 * @file    nanoc_lexer.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_lexer.h"
#include "nanoc_lexer_literal.h"
#include "nanoc_lexer_identifier.h"
#include "nanoc_lexer_number.h"
#include "nanoc_lexer_operator.h"
#include <ctype.h>

static void nanoc_lexer_read(NanoC_Lexer *lexer)
{
	lexer->Current = *lexer->Ptr;
	if(lexer->Current == '\n')
	{
		++lexer->Pos.Row;
		lexer->Pos.Col = -1;
		lexer->LineBegin = lexer->Ptr + 1;
	}
}

NanoC_Char nanoc_lexer_advance(NanoC_Lexer *lexer)
{
	if(lexer->Current == '\0')
	{
		return '\0';
	}

	if(lexer->Current == '\t')
	{
		lexer->Pos.Col = (lexer->Pos.Col + 4) & ~0x3;
	}
	else
	{
		++lexer->Pos.Col;
	}

	++lexer->Ptr;
	nanoc_lexer_read(lexer);
	return lexer->Current;
}

static void nanoc_lexer_multi_line_comment(NanoC_Lexer *lexer)
{
	NanoC_Char c;
	nanoc_lexer_advance(lexer);
	for(;;)
	{
		c = nanoc_lexer_advance(lexer);
		if(!c || (c == '*' && lexer->Ptr[1] == '/'))
		{
			break;
		}
	}

	nanoc_lexer_advance(lexer);
	nanoc_lexer_advance(lexer);
}

static void nanoc_lexer_single_line_comment(NanoC_Lexer *lexer)
{
	nanoc_lexer_advance(lexer);
	while(nanoc_lexer_advance(lexer) != '\n') {}
	nanoc_lexer_advance(lexer);
}

static NanoC_Bool nanoc_lexer_skip(NanoC_Lexer *lexer)
{
	NanoC_Char cur = lexer->Current;
	if(cur == '/')
	{
		NanoC_Char next = lexer->Ptr[1];
		if(next == '*')
		{
			nanoc_lexer_multi_line_comment(lexer);
			return 1;
		}
		else if(next == '/')
		{
			nanoc_lexer_single_line_comment(lexer);
			return 1;
		}
	}
	else if(isspace(cur))
	{
		nanoc_lexer_advance(lexer);
		return 1;
	}

	return 0;
}

void nanoc_lexer_init(NanoC_Lexer *lexer, const char *src, char *strings)
{
	lexer->Pos.Col = 0;
	lexer->Pos.Row = 1;
	lexer->Ptr = src;
	lexer->LineBegin = src;
	lexer->Strings = strings;
	nanoc_lexer_read(lexer);
}

void nanoc_lexer_forward(NanoC_Lexer *lexer, const char *p)
{
	while(lexer->Ptr < p)
	{
		nanoc_lexer_advance(lexer);
	}
}

void nanoc_lexer_next(NanoC_Lexer *lexer, NanoC_Token *token)
{
	while(nanoc_lexer_skip(lexer)) {}
	token->Pos = lexer->Pos;
	token->Ptr = lexer->Ptr;
	token->LineBegin = lexer->LineBegin;
	if(lexer->Current == '\0')
	{
		token->Type = NANOC_TT_NULL;
		return;
	}

	if(nanoc_lexer_identifier(lexer, token)) { return; }
	if(nanoc_lexer_number(lexer, token)) { return; }
	if(nanoc_lexer_char(lexer, token)) { return; }
	if(nanoc_lexer_string(lexer, token)) { return; }
	nanoc_lexer_operator(lexer, token);
}
