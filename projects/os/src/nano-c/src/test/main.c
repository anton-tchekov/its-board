#ifdef NANOC_DESKTOP

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "file_io.h"
#include "nanoc_parser.h"
#include "nanoc_expression.h"
#include "nanoc_disasm.h"
#include "nanoc_debug.h"
#include "nanoc_instruction.h"
#include "nanoc_interpreter.h"
#include "nanoc_builtin.h"
#include "nanoc_test.h"

#include "builtins.h"

static NanoC_Value debug_print(NanoC_Value a, NanoC_Value *p)
{
	printf("%"PRIdPTR"\n", p[0]);
	return 0;
	(void)a;
}

static NanoC_Value pchr(NanoC_Value a, NanoC_Value *p)
{
	fputc(p[0], stdout);
	return 0;
	(void)a;
}

int main(int argc, char **argv)
{
	const char *filename;
	size_t length;
	NanoC_Value rv;
	char *content;
	NanoC_Parser parser;
	NanoC_Status ret;
	u8 output_buf[1024];
	char strings[1024];

	NanoC_Value (*functions[])(NanoC_Value, NanoC_Value *) = { debug_print, pchr, pchr };

	NanoC_Builtins builtins =
	{
		3,
		functions
	};

	nanoc_test_run();

	if(argc != 2)
	{
		fprintf(stderr, "Usage: ./nano-c `sourcefile.uhc`\n");
		return 1;
	}

	filename = argv[1];
	if(!(content = file_get_contents(filename, FILE_IO_MODE_TEXT, &length)))
	{
		fprintf(stderr, "Failed to open source file \"%s\"\n", filename);
		return 1;
	}

	nanoc_parser_init(&parser, content, strings,
		output_buf, sizeof(output_buf), &parser_builtins);

	if((ret = nanoc_file(&parser)))
	{
		nanoc_token_print(nanoc_tokenstream_get(&parser.TokenStream, 0));
		return 1;
	}

	nanoc_disasm(parser.Output.Buffer, parser.Output.Pos);

	printf("interpreter status = %s\n", nanoc_status_message(
		nanoc_interpreter_run(parser.Output.Buffer, &builtins, &rv)));

#if 0
	printf("%s\n", content);
	nanoc_lexer_init(&lexer, content);

	do
	{
		nanoc_lexer_next(&lexer, &token);
		nanoc_token_print(&token);
	}
	while(token.Type != NANOC_TT_NULL);
#endif

	free(content);


	return 0;
}

#endif /* NANOC_DESKTOP */
