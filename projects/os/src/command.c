#include "command.h"
#include "stdio.h"
#include "shell.h"
#include "nanoc.h"

#define STRINGS_OFFSET (2 * 1024)

static int _putd(int *parameters, uint8_t *heap)
{
	char buf[128];
	sprintf(buf, "%d\n", parameters[0]);
	shell_print(buf);
	return 0;
}

static int _puts(int *parameters, uint8_t *heap)
{
	char buf[128];
	sprintf(buf, "%s\n", (char *)&heap[parameters[0]]);
	shell_print(buf);
	return 0;
}

static int _about(int *parameters, uint8_t *heap)
{
	shell_print(
		"ITS Board Shell V0.1\n"
		"\n"
		"Created by Anton Tchekov\n"
		"/ September 2023 /\n");
	return 0;
}

static void compile(const char *src, int length)
{
	NanoC nanoc;
	uint8_t program[4 * 1024], data[4 * 1024];
	char buf[128];
	int ret;

	nanoc_init(&nanoc);
	nanoc_add_function(&nanoc, 0, "putd",  1, _putd);
	nanoc_add_function(&nanoc, 1, "puts",  1, _puts);
	nanoc_add_function(&nanoc, 2, "about", 0, _about);
	ret = nanoc_compile(&nanoc, src, program,
		data + STRINGS_OFFSET, STRINGS_OFFSET);

	if(ret)
	{
		int i;
		const char *s;
		sprintf(buf, "parse error: %s\n"
				"row: %d, col: %d\n",
				nanoc_error_message(ret),
				nanoc.Token.Pos.Row, nanoc.Token.Pos.Col);
		shell_print(buf);

		for(i = 0, s = nanoc.Lexer.LineBegin; *s != '\n' &&
			s - src < length; ++s, ++i)
		{
			if(i == nanoc.Token.Pos.Col)
			{
				shell_fg(TERMINAL_BRIGHT_RED);
			}

			if(i == nanoc.Lexer.Pos.Col)
			{
				shell_fg(TERMINAL_WHITE);
			}

			shell_char(*s);
		}

		shell_fg(TERMINAL_WHITE);
		shell_char('\n');
		return;
	}

	if((ret = nanoc_run(&nanoc, program, data)))
	{
		sprintf(buf, "runtime error: %s\n", nanoc_error_message(ret));
		shell_print(buf);
	}
}

void command_run(const char *cmd, int len)
{
	compile(cmd, len);
}
