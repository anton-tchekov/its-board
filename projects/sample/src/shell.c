/*

Shell Idea Plan:

everythung is a expression
commands are c code - REPL
edit("function");
exit editor:
	function compiles
	function does not compile
	error propagation
		function cannot be run because it contains errors
		function "print_str" cannot be run due to error in function "print_char"


*/

#include "shell.h"
#include "terminal.h"
#include "string.h"
//#include "ctype.h"
#include "ps2.h"
#include "nanoc.h"
#include "error.h"

#include "stdio.h"

#define ARRLEN(X) (sizeof(X) / sizeof(*X))

/* Current working directory */
static char wd[256];

/* Current command */
static char cmd[256];
static int idx;

static const char *notice =
	"ITS-Board OS Shell V0.1\n";

typedef struct
{
	const char *Name;
	void (*FN)(void);
} Command;

void cmd_append(int c)
{
	cmd[idx++] = c;
	cmd[idx] = '\0';
}

void cmd_ls(void)
{
}

void cmd_pwd(void)
{
	terminal_print(wd);
	terminal_print("\n");
}

void cmd_neofetch(void)
{
	terminal_print(
		"###  #####  ### \n"
		" #     #   #    \n"
		" #     #    ### \n"
		" #     #       #\n"
		"###    #    ### \n"
		"                    ITS BOARD OS V0.1\n"
	);
}

const Command commands[] =
{
	{ "neofetch", cmd_neofetch },
	{ "ls", cmd_ls },
	{ "pwd", cmd_pwd },
};

#define NUM_COMMANDS ARRLEN(commands)

static inline int isprint(int c)
{
	return c >= 32 && c <= 126;
}

void shell_prompt(void)
{
	terminal_print("$ ");
}

void shell_init(void)
{
	idx = 0;
	cmd[0] = '\0';
	strcpy(wd, "/");
	terminal_print(notice);
	shell_prompt();
}

#define STRINGS_OFFSET (2 * 1024)

int print_number(int *parameters, uint8_t *heap)
{
	char buf[128];
	sprintf(buf, "%d\n", parameters[0]);
	terminal_print(buf);
	return 0;
}

int print_string(int *parameters, uint8_t *heap)
{
	char buf[128];
	sprintf(buf, "%s\n", (char *)&heap[parameters[0]]);
	terminal_print(buf);
	return 0;
}

int fn_clear(int *parameters, uint8_t *heap)
{
	terminal_clear();
	return 0;
}


void compile(void)
{
	NanoC nanoc;
	uint8_t program[4*1024], data[4*1024];
	char buf[128];
	int ret;
	int length = strlen(cmd);
	char *src = cmd;

	/* init NanoC */
	nanoc_init(&nanoc);
	nanoc_add_function(&nanoc, 0, "putd",  1, print_number);
	nanoc_add_function(&nanoc, 1, "puts",  1, print_string);
	nanoc_add_function(&nanoc, 2, "clear", 0, fn_clear);

	if((ret = nanoc_compile(&nanoc, src, program, data + STRINGS_OFFSET, STRINGS_OFFSET)))
	{
		int i;
		const char *s;
		sprintf(buf, "parse error: %s\n"
				"row: %d, col: %d\n",
				nanoc_error_message(ret),
				nanoc.Token.Pos.Row, nanoc.Token.Pos.Col);
		terminal_print(buf);

		for(i = 0, s = nanoc.Lexer.LineBegin; *s != '\n' && s - src < length; ++s, ++i)
		{
			if(i == nanoc.Token.Pos.Col)
			{
				terminal_fg(ID_BRIGHT_RED);
			}

			if(i == nanoc.Lexer.Pos.Col)
			{
				terminal_fg(ID_WHITE);
			}

			terminal_char(*s);
		}

		terminal_fg(ID_WHITE);
		terminal_print("\n");
		return;
	}

	if((ret = nanoc_run(&nanoc, program, data)))
	{
		sprintf(buf, "runtime error: %s\n", nanoc_error_message(ret));
		terminal_print(buf);
	}
}






void run_command(void)
{
	compile();
	/*int i;
	for(i = 0; i < NUM_COMMANDS && strcmp(cmd, commands[i].Name); ++i) {}
	if(i == NUM_COMMANDS)
	{
		terminal_print("Command not found!\n");
	}
	else
	{
		commands[i].FN();
	}*/
}

void shell_update(void)
{
	KeyEvent event;
	int c;
	if(!ps2_read(&event))
	{
		return;
	}
	if(event.State != KEYSTATE_PRESSED)
	{
		return;
	}

	c = event.Codepoint;
	if(c == '\n')
	{
		terminal_print("\n");
		run_command();
		shell_prompt();
		idx = 0;
		cmd[0] = '\0';
	}
	else if(isprint(c))
	{
		char buf[2];
		buf[0] = event.Codepoint;
		buf[1] = '\0';
		terminal_print(buf);
		cmd_append(c);
	}
}
