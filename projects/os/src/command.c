#include "command.h"
#include "shell.h"
#include "nanoc_parser.h"
#include "nanoc_instruction.h"
#include "nanoc_interpreter.h"
#include "nanoc_status.h"

#include <stdio.h>
#include <string.h>

static i32r debug_print(i32r a, i32 *p)
{
	char buf[128];
	sprintf(buf, "%d\n", p[0]);
	shell_print(buf);
	return 0;
}

static i32r _puts(i32r a, i32 *p)
{
	char buf[128];
	sprintf(buf, "%s\n", (char *)p[0]);
	shell_print(buf);
	return 0;
}

static i32r _about(i32r a, i32 *p)
{
	shell_print(
		"ITS Board Shell V0.1\n"
		"\n"
		"Created by Anton Tchekov\n"
		"/ September 2023 /\n");
	return 0;
}

static i32r _printf(i32r a, i32 *p)
{
	char buf[128];
	char *format = (char *)p[0];

	/* couldn't find a better way to do this */
	switch(a)
	{
		case 0: sprintf(buf, format);
			break;
		case 1: sprintf(buf, format, p[1]);
			break;
		case 2: sprintf(buf, format, p[1], p[2]);
			break;
		case 3: sprintf(buf, format, p[1], p[2], p[3]);
			break;
		case 4: sprintf(buf, format, p[1], p[2], p[3], p[4]);
			break;
		case 5: sprintf(buf, format, p[1], p[2], p[3], p[4], p[5]);
			break;
		case 6: sprintf(buf, format, p[1], p[2], p[3], p[4], p[5], p[6]);
			break;
		default: strcpy(buf,
			"nobody will ever call printf with\n"
			"more than like 6 parameters, right?\n"
			"\n"
			"~ Anton, 2023\n");
			break;
	}

	shell_print(buf);
	return 0;
}

static void compile(const char *src, int length)
{
	NanoC_Status status;
	NanoC_Parser parser;
	u8 output_buf[1024];

	i32r (*functions[])(i32r, i32 *) =
	{
		debug_print
	};

	NanoC_Builtins builtins =
	{
		1,
		functions
	};

	nanoc_parser_init(&parser, src, output_buf, sizeof(output_buf));
	status = nanoc_statement(&parser);
	if(status)
	{
		char buf[128];
		sprintf(buf, "\nParse error: %s\n", nanoc_status_message(status));
		shell_print(buf);
		return;
	}

	nanoc_output_emit(&parser.Output, NANOC_INSTR_HALT);

	status = nanoc_interpreter_run(parser.Output.Buffer, &builtins);
	if(status)
	{
		char buf[128];
		sprintf(buf, "\nRuntime error: %s\n",
			nanoc_status_message(status));

		shell_print(buf);
		return;
	}

	shell_print("\n\nREADY.\n");
}

void command_run(const char *cmd, int len)
{
	compile(cmd, len);
}
