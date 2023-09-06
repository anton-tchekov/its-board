#ifdef NANOC_DESKTOP

#include "nanoc_token.h"
#include "nanoc_debug.h"
#include "nanoc_tokentype.h"
#include "nanoc_instruction.h"
#include "nanoc_terminal.h"
#include <stdio.h>

size_t _print_handle_tab(NanoC_Char c, size_t count)
{
	if(c == '\t')
	{
		do
		{
			fputc(' ', stdout);
			++count;
		}
		while((count & 0x3) != 0);
	}
	else
	{
		fputc(c, stdout);
		++count;
	}

	return count;
}

void nanoc_token_print(NanoC_Token *token)
{
	size_t i, before, count;
	const char *s;

	printf("%3d:%3d | %s", token->Pos.Row, token->Pos.Col,
		nanoc_tokentype_tostring(token->Type));

	if(token->Type == NANOC_TT_INTEGER)
	{
		printf(" (value = %d)", token->Value.Integer);
	}

	fputc('\n', stdout);
	if(token->Type == NANOC_TT_NULL)
	{
		return;
	}

	/* Print part of line before token */
	count = 0;
	for(s = token->LineBegin; s < token->Ptr; ++s)
	{
		count = _print_handle_tab(*s, count);
	}

	/* Print token */
	before = count;
	printf(COLOR_BOLD_RED "%.*s" COLOR_RESET, (int)token->Length, token->Ptr);
	count += token->Length;
	s += token->Length;

	/* Print part of line after token */
	for(; *s && *s != '\n'; ++s)
	{
		count = _print_handle_tab(*s, count);
	}
	fputc('\n', stdout);

	/* Print spaces before underline */
	for(i = 0; i < before; ++i)
	{
		fputc(' ', stdout);
	}

	/* Print underline */
	fputs(COLOR_BOLD_RED "^", stdout);
	for(i = 1; i < token->Length; ++i)
	{
		fputc('~', stdout);
	}

	fputs(COLOR_RESET "\n", stdout);
}

#endif /* NANOC_DESKTOP */
