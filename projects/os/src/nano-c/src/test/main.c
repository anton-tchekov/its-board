#ifdef NANOC_DESKTOP

#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"
#include "nanoc_parser.h"
#include "nanoc_expression.h"
#include "nanoc_disasm.h"
#include "nanoc_debug.h"
#include "nanoc_instruction.h"
#include "nanoc_interpreter.h"
#include "nanoc_builtin.h"
#include "nanoc_test.h"

static const char *builtin_names =
	"";

static i32 debug_print(i32r a, i32 *p)
{
	printf("%d\n", p[0]);
	return 0;
}

int main(int argc, char **argv)
{
	const char *filename;
	size_t length;
	char *content;
	NanoC_Parser parser;
	u8 output_buf[1024];

	i32r (*functions[])(i32r, i32r *) = { debug_print };

	NanoC_Builtins builtins =
	{
		1,
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

	nanoc_parser_init(&parser, content, output_buf, sizeof(output_buf),
		builtin_names);

	nanoc_statement(&parser);
	nanoc_output_emit(&parser.Output, NANOC_INSTR_HALT);
	nanoc_disasm(parser.Output.Buffer, parser.Output.Pos);

	printf("exit code = %d\n",
		nanoc_interpreter_run(parser.Output.Buffer, &builtins));

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
