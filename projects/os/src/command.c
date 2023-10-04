/**
 * @file    command.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "command.h"
#include "shell.h"
#include "nanoc_parser.h"
#include "nanoc_instruction.h"
#include "nanoc_interpreter.h"
#include "nanoc_status.h"
#include "nanoc_ctype.h"
#include "util.h"
#include "delay.h"
#include "editor.h"
#include "ctype_ext.h"
#include "ff.h"
#include "ffstatus.h"

#include "stm32f4xx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _not_implemented(void)
{
	shell_print("Not Implemented\n");
}

static i32r _printn(i32r a, i32 *p)
{
	char buf[16];
	sprintf(buf, "%d\n", p[0]);
	shell_print(buf);
	return 0;
	(void)a;
}

static i32r _printc(i32r a, i32 *p)
{
	char buf[2] = { p[0], '\0' };
	shell_print(buf);
	return 0;
	(void)a;
}

static i32r _prints(i32r a, i32 *p)
{
	shell_print((char *)p[0]);
	shell_char('\n');
	return 0;
	(void)a;
}

static i32r _about(i32r a, i32 *p)
{
	shell_print(
		"ITS Board Shell V0.1\n"
		"\n"
		"Created by Anton Tchekov\n"
		"/ September 2023 /\n");
	return 0;
	(void)a, (void)p;
}

static i32r _delay_ms(i32r a, i32 *p)
{
	delay_ms(p[0]);
	return 0;
	(void)a;
}

static GPIO_TypeDef *_getport(int chr)
{
	switch(chr)
	{
	case 'D':
		return GPIOD;

	case 'E':
		return GPIOE;
	}

	return NULL;
}

static int check_pin_fn(GPIO_TypeDef *gpio, int port)
{
	if(!gpio)
	{
		shell_print("Invalid port!");
		return 1;
	}

	if(port < 0 || port > 16)
	{
		shell_print("Invalid pin number!");
		return 1;
	}

	return 0;
}

static i32r _pin_high(i32r a, i32 *p)
{
	GPIO_TypeDef *gpio = _getport(p[0]);
	int port = p[1];
	if(check_pin_fn(gpio, port)) { return 1; }
	gpio->BSRR = (1 << port);
	return 0;
	(void)a;
}

static i32r _pin_low(i32r a, i32 *p)
{
	GPIO_TypeDef *gpio = _getport(p[0]);
	int port = p[1];
	if(check_pin_fn(gpio, port)) { return 1; }
	gpio->BSRR = (1 << (16 + port));
	return 0;
	(void)a;
}

static i32r _pin_toggle(i32r a, i32 *p)
{
	GPIO_TypeDef *gpio = _getport(p[0]);
	int port = p[1];
	if(check_pin_fn(gpio, port)) { return 1; }
	gpio->ODR ^= (1 << p[1]);
	return 0;
	(void)a;
}

static i32r _max(i32r a, i32 *p)
{
	return p[0] > p[1] ? p[0] : p[1];
	(void)a;
}

static i32r _min(i32r a, i32 *p)
{
	return p[0] < p[1] ? p[0] : p[1];
	(void)a;
}

static i32r _abs(i32r a, i32 *p)
{
	i32 v = p[0];
	return v < 0 ? -v : v;
	(void)a;
}

static i32r _sign(i32r a, i32 *p)
{
	i32 v = p[0];
	if(v == 0)
	{
		return 0;
	}

	return (v < 0) ? -1 : 1;
	(void)a;
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

static i32r _mkfs(i32r a, i32 *p)
{
	u8 buf[512];
	return f_mkfs((char *)p[0], 0, buf, sizeof(buf));
	(void)a;
}

static i32r _mkdir(i32r a, i32 *p)
{
	int i;
	for(i = 0; i < a; ++i)
	{
		int ret = f_mkdir((char *)p[i]);
		if(ret)
		{
			return ret;
		}
	}

	return 0;
}

static i32r _create(i32r a, i32 *p)
{
	FIL fp;
	int ret = f_open(&fp, (char *)p[0], FA_CREATE_NEW);
	if(ret) { return ret; }
	return f_close(&fp);
	(void)a;
}

static i32r _rm(i32r a, i32 *p)
{
	int i;
	for(i = 0; i < a; ++i)
	{
		int ret = f_unlink((char *)p[i]);
		if(ret)
		{
			return ret;
		}
	}

	return 0;
}

static i32r _mv(i32r a, i32 *p)
{
	return f_rename((char *)p[1], (char *)p[0]);
	(void)a;
}

static i32r _cp(i32r a, i32 *p)
{
	_not_implemented();
	return 0;
	(void)a, (void)p;
}

static i32r _ls(i32r a, i32 *p)
{
	return 0;
	(void)a;
}

static i32r _edit(i32r a, i32 *p)
{
	editor_load((char *)p[0]);
	return 0;
	(void)a;
}

static i32r _fs_strerror(i32r a, i32 *p)
{
	return (i32r)f_status_str(p[0]);
	(void)a;
}

static i32r _rand(i32r a, i32 *p)
{
	return rand();
	(void)a, (void)p;
}

static i32r _srand(i32r a, i32 *p)
{
	srand(p[0]);
	return 0;
	(void)a;
}

static i32r _sprintf(i32r a, i32 *p)
{
	_not_implemented();
	return 0;
	(void)a, (void)p;
}

static i32r _snprintf(i32r a, i32 *p)
{
	_not_implemented();
	return 0;
	(void)a, (void)p;
}

static i32r _memcpy(i32r a, i32 *p)
{
	return (i32r)memcpy((void *)p[0], (void *)p[1], p[2]);
	(void)a;
}

static i32r _memset(i32r a, i32 *p)
{
	return (i32r)memset((void *)p[0], p[1], p[2]);
	(void)a;
}

static i32r _memcmp(i32r a, i32 *p)
{
	return (i32r)memcmp((void *)p[0], (void *)p[1], p[2]);
	(void)a;
}

static i32r _memmove(i32r a, i32 *p)
{
	return (i32r)memmove((void *)p[0], (void *)p[1], p[2]);
	(void)a;
}

static i32r _strlen(i32r a, i32 *p)
{
	return strlen((char *)p[0]);
	(void)a;
}

static i32r _strnlen(i32r a, i32 *p)
{
	return strnlen((char *)p[0], p[1]);
	(void)a;
}

static i32r _strcpy(i32r a, i32 *p)
{
	return (i32r)strcpy((char *)p[0], (char *)p[1]);
	(void)a;
}

static i32r _strncpy(i32r a, i32 *p)
{
	return (i32r)strncpy((char *)p[0], (char *)p[1], p[2]);
	(void)a;
}

static i32r _strcmp(i32r a, i32 *p)
{
	return strcmp((char *)p[0], (char *)p[1]);
	(void)a;
}

static i32r _strncmp(i32r a, i32 *p)
{
	return strncmp((char *)p[0], (char *)p[1], p[2]);
	(void)a;
}

static i32r _strchr(i32r a, i32 *p)
{
	return (i32r)strchr((char *)p[0], p[1]);
	(void)a;
}

static i32r _clipget(i32r a, i32 *p)
{
	_not_implemented();
	return 0;
	(void)a, (void)p;
}

static i32r _clipsave(i32r a, i32 *p)
{
	_not_implemented();
	return 0;
	(void)a;
}

static i32r _isoct(i32r a, i32 *p)
{
	return is_octal(p[0]);
	(void)a, (void)p;
}

static i32r _isbin(i32r a, i32 *p)
{
	return is_binary(p[0]);
	(void)a;
}

static i32r _clear(i32r a, i32 *p)
{
	shell_cls();
	return 0;
	(void)a, (void)p;
}

static i32r _gotoxy(i32r a, i32 *p)
{
	shell_xy(p[0], p[1]);
	return 0;
	(void)a;
}

static i32r _hexdump(i32r a, i32 *p)
{


	return 0;
	(void)a;
}

static i32r _help(i32r a, i32 *p);

static const NanoC_ParserBuiltin parser_builtins_data[] =
{
	{ .Name = "printn",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "printc",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "prints",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "printf",        .NumArgs = 1, .IsVariadic = 1 },

	{ .Name = "sprintf",       .NumArgs = 1, .IsVariadic = 1 },
	{ .Name = "snprintf",      .NumArgs = 1, .IsVariadic = 1 },

	{ .Name = "clear",         .NumArgs = 0, .IsVariadic = 0 },
	{ .Name = "gotoxy",        .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "about",         .NumArgs = 0, .IsVariadic = 0 },
	{ .Name = "help",          .NumArgs = 0, .IsVariadic = 0 },

	{ .Name = "delay_ms",      .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "pin_high",      .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "pin_low",       .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "pin_toggle",    .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "isupper",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "islower",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isdigit",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isalpha",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isalnum",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isprint",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isspace",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isxdigit",      .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "tolower",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "toupper",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isbin",         .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "isoct",         .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "max",           .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "min",           .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "abs",           .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "sign",          .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "rand",          .NumArgs = 0, .IsVariadic = 0 },
	{ .Name = "srand",         .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "mkfs",          .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "mkdir",         .NumArgs = 1, .IsVariadic = 1 },
	{ .Name = "create",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "rm",            .NumArgs = 1, .IsVariadic = 1 },
	{ .Name = "mv",            .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "cp",            .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "ls",            .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "edit",          .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "fs_strerror",   .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "memcpy",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memset",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memcmp",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memmove",       .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "strlen",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "strnlen",       .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strcpy",        .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strncpy",       .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "strcmp",        .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strncmp",       .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "strchr",        .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "clipget",       .NumArgs = 0, .IsVariadic = 0 },
	{ .Name = "clipsave",      .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "hexdump",       .NumArgs = 2, .IsVariadic = 0 },
};

static const NanoC_ParserBuiltins parser_builtins =
{
	.Count = ARRLEN(parser_builtins_data),
	.Table = parser_builtins_data
};

#define HELP_COLS      4
#define HELP_COLWIDTH 15

static i32r _help(i32r a, i32 *p)
{
	int i;
	char buf[16];
	const NanoC_ParserBuiltin *builtin = parser_builtins_data;

	for(i = 0; i < ARRLEN(parser_builtins_data); ++i, ++builtin)
	{
		buf[0] = builtin->IsVariadic ? '>' : ' ';
		buf[1] = builtin->NumArgs + '0';
		buf[2] = ' ';
		strcpy(buf + 3, builtin->Name);
		shell_xy((i % HELP_COLS) * HELP_COLWIDTH, i / HELP_COLS + 1);
		shell_print(buf);
	}

	return 0;
	(void)a, (void)p;
}

static i32r (*const functions[])(i32r, i32 *) =
{
	_printn,
	_printc,
	_prints,
	_printf,

	_sprintf,
	_snprintf,

	_clear,
	_gotoxy,

	_about,
	_help,

	_delay_ms,
	_pin_high,
	_pin_low,
	_pin_toggle,

	nanoc_isupper,
	nanoc_islower,
	nanoc_isdigit,
	nanoc_isalpha,
	nanoc_isalnum,
	nanoc_isprint,
	nanoc_isspace,
	nanoc_isxdigit,
	nanoc_tolower,
	nanoc_toupper,
	_isbin,
	_isoct,

	_max,
	_min,
	_abs,
	_sign,

	_rand,
	_srand,

	_mkfs,
	_mkdir,
	_create,
	_rm,
	_mv,
	_cp,
	_ls,
	_edit,
	_fs_strerror,

	_memcpy,
	_memset,
	_memcmp,
	_memmove,
	_strlen,
	_strnlen,
	_strcpy,
	_strncpy,
	_strcmp,
	_strncmp,
	_strchr,

	_clipget,
	_clipsave,

	_hexdump,
};

static const NanoC_Builtins builtins = { ARRLEN(functions), functions };

static void compile(const char *src, int length)
{
	NanoC_Status status;
	NanoC_Parser parser;
	u8 output_buf[1024];

	nanoc_parser_init(&parser, src, output_buf, sizeof(output_buf),
		&parser_builtins);
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
