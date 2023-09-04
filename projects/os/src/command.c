#include "command.h"
#include "shell.h"
#include "nanoc.h"

#include <stdio.h>
#include <string.h>

#define STRINGS_OFFSET (2 * 1024)

static int _putd(int a, int *p)
{
	char buf[128];
	sprintf(buf, "%d\n", p[0]);
	shell_print(buf);
	return 0;
}

static int _puts(int a, int *p)
{
	char buf[128];
	sprintf(buf, "%s\n", (char *)p[0]);
	shell_print(buf);
	return 0;
}

static int _about(int a, int *p)
{
	shell_print(
		"ITS Board Shell V0.1\n"
		"\n"
		"Created by Anton Tchekov\n"
		"/ September 2023 /\n");
	return 0;
}

static int _printf(int a, int *p)
{
	int i;
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
	NanoC nanoc;
	uint8_t program[4 * 1024], data[4 * 1024];
	char buf[128];
	int ret;

	nanoc_init(&nanoc);
	nanoc_add_function(&nanoc, 0, "putd",  1, _putd);
	nanoc_add_function(&nanoc, 1, "puts",  1, _puts);
	nanoc_add_function(&nanoc, 2, "about", 0, _about);
	nanoc_add_function(&nanoc, 3, "printf", 0xFF, _printf);
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
