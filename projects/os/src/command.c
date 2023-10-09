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
#include "util.h"
#include "delay.h"
#include "editor.h"
#include "ctype_ext.h"
#include "clipboard.h"
#include "ff.h"
#include "ffstatus.h"
#include "sd/sd.h"
#include "ramdisk.h"

#include "stm32f4xx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FATFS _fatfs[2];
SD sd;

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
	case 'D': return GPIOD;
	case 'E': return GPIOE;
	case 'F': return GPIOF;
	case 'G': return GPIOG;
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
	gpio->ODR ^= (1 << port);
	return 0;
	(void)a;
}

static i32r _pin_get(i32r a, i32 *p)
{
	GPIO_TypeDef *gpio = _getport(p[0]);
	int port = p[1];
	if(check_pin_fn(gpio, port)) { return 1; }
	return (gpio->IDR >> port) & 1;
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

static i32r _pow(i32r a, i32 *p)
{
	int base = p[0];
	int exp = p[1];
	int res = 1;
	while(exp > 0)
	{
		res *= base;
		--exp;
	}

	return res;
	(void)a;
}

static int abomination(int count, char *buf, size_t n, const char *format, i32 *p)
{
	/* couldn't find a better way to do this */
	switch(count)
	{
		case 0: return snprintf(buf, n, format);
		case 1: return snprintf(buf, n, format, p[0]);
		case 2: return snprintf(buf, n, format, p[0], p[1]);
		case 3: return snprintf(buf, n, format, p[0], p[1], p[2]);
		case 4: return snprintf(buf, n, format, p[0], p[1], p[2], p[3]);
		case 5: return snprintf(buf, n, format, p[0], p[1], p[2], p[3], p[4]);
		case 6: return snprintf(buf, n, format, p[0], p[1], p[2], p[3], p[4], p[5]);
	}

	return -1;
}

static i32r _printf(i32r a, i32 *p)
{
	int ret;
	char buf[256];
	if(a > 6)
	{
		shell_print("nobody will ever call printf with\n"
			"more than like 6 parameters, right?\n");
		return -1;
	}

	ret = abomination(a, buf, sizeof(buf), (char *)p[0], p + 1);
	shell_print(buf);
	return ret;
}

static i32r _sprintf(i32r a, i32 *p)
{
	return abomination(a, (char *)p[0], 0xFFFFFF, (char *)p[1], p + 2);
}

static i32r _snprintf(i32r a, i32 *p)
{
	return abomination(a, (char *)p[0], p[1], (char *)p[2], p + 3);;
}

static int drive_valid(int drive)
{
	return drive >= 0 && drive <= 1;
}

void fserror(int ret)
{
	shell_print(f_status_str(ret));
	shell_char('\n');
}

static i32r _blkinfo(i32r a, i32 *p)
{
	int drive = p[0];
	if(!drive_valid(drive))
	{
		fserror(FR_INVALID_DRIVE);
		return 1;
	}

	switch(drive)
	{
	case 0:
		ramdisk_info_print();
		break;

	case 1:
		sd_info_print(&sd);
		break;
	}

	return 0;
	(void)a;
}

static i32r _mkfs(i32r a, i32 *p)
{
	int drive = p[0];
	if(!drive_valid(drive)) { return FR_INVALID_DRIVE; }
	char path[3] = { drive + '0', ':', '\0' };
	u8 buf[512];
	return f_mkfs(path, 0, buf, sizeof(buf));
	(void)a;
}

static i32r _mount(i32r a, i32 *p)
{
	int drive = p[0];
	if(!drive_valid(drive)) { return FR_INVALID_DRIVE; }
	char path[3] = { drive + '0', ':', '\0' };
	return f_mount(_fatfs + drive, path, 1);
	(void)a;
}

static i32r _unmount(i32r a, i32 *p)
{
	int drive = p[0];
	if(!drive_valid(drive)) { return FR_INVALID_DRIVE; }
	char path[3] = { drive + '0', ':', '\0' };
	return f_unmount(path);
	(void)a;
}

static i32r _mkdir(i32r a, i32 *p)
{
	int i, ret;
	for(i = 0; i < a; ++i)
	{
		ret = f_mkdir((char *)p[i]);
		if(ret) { return ret; }
	}

	return 0;
}

static i32r _create(i32r a, i32 *p)
{
	FIL fp;
	int i, ret;
	for(i = 0; i < a; ++i)
	{
		ret = f_open(&fp, (char *)p[0], FA_CREATE_NEW);
		if(ret) { return ret; }
		ret = f_close(&fp);
		if(ret) { return ret; }
	}

	return 0;
}

static i32r _rm(i32r a, i32 *p)
{
	int i, ret;
	for(i = 0; i < a; ++i)
	{
		ret = f_unlink((char *)p[i]);
		if(ret) { return ret; }
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
	int ret;
	DIR dp;
	FILINFO fno;
	char buf[32];

	ret = f_opendir(&dp, (char *)p[0]);
	if(ret)
	{
		fserror(ret);
		return 1;
	}

	for(;;)
	{
		ret = f_readdir(&dp, &fno);
		if(ret)
		{
			fserror(ret);
			return ret;
		}

		if(fno.fname[0] == 0)
		{
			break;
		}

		if(fno.fattrib & AM_DIR)
		{
			sprintf(buf, "%s/\n", fno.fname);
		}
		else
		{
			sprintf(buf, "%-15s %10u\n", fno.fname, fno.fsize);
		}

		shell_print(buf);
	}

	ret = f_closedir(&dp);
	if(ret)
	{
		fserror(ret);
		return ret;
	}

	return 0;
	(void)a;
}

static i32r _edit(i32r a, i32 *p)
{
	editor_load_cmd((char *)p[0]);
	return 0;
	(void)a;
}

static i32r _edsav(i32r a, i32 *p)
{
	editor_save_cmd((char *)p[0]);
	return 0;
	(void)a;
}

static i32r _fserr(i32r a, i32 *p)
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

static i32r _memchr(i32r a, i32 *p)
{
	return (i32r)memchr((void *)p[0], p[1], p[2]);
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
	if(a == 0)
	{
		int val;
		return (i32r)clipboard_get(&val);
	}
	else if(a == 1)
	{
		return (i32r)clipboard_get((int *)p[0]);
	}

	return 1;
}

static i32r _clipsave(i32r a, i32 *p)
{
	return clipboard_save((char *)p[0], p[1]);
	(void)a;
}

static i32r nanoc_isupper(i32r a, i32 *p)
{
	return isupper(p[0]);
	(void)a;
}

static i32r nanoc_islower(i32r a, i32 *p)
{
	return islower(p[0]);
	(void)a;
}

static i32r nanoc_isdigit(i32r a, i32 *p)
{
	return isdigit(p[0]);
	(void)a;
}

static i32r nanoc_isalpha(i32r a, i32 *p)
{
	return isalpha(p[0]);
	(void)a;
}

static i32r nanoc_isalnum(i32r a, i32 *p)
{
	return isalnum(p[0]);
	(void)a;
}

static i32r nanoc_isprint(i32r a, i32 *p)
{
	return isprint(p[0]);
	(void)a;
}

static i32r nanoc_isspace(i32r a, i32 *p)
{
	return isspace(p[0]);
	(void)a;
}

static i32r nanoc_isxdigit(i32r a, i32 *p)
{
	return isxdigit(p[0]);
	(void)a;
}

static i32r nanoc_tolower(i32r a, i32 *p)
{
	return tolower(p[0]);
	(void)a;
}

static i32r nanoc_toupper(i32r a, i32 *p)
{
	return toupper(p[0]);
	(void)a;
}

static i32r _isoct(i32r a, i32 *p)
{
	return is_octal(p[0]);
	(void)a;
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

static inline int peek(u32 addr)
{
	return *(u8 *)addr;
}

static void hexdump(u32 start, u32 len)
{
	u32 s = start & ~7;
	u32 end = start + len;
	u32 e = (end + 7) & ~7;
	char buf[16];

	shell_print("\n             +0 +1 +2 +3 +4 +5 +6 +7\n");
	for(; s < e; s += 8)
	{
		u32 cs;
		u32 ce = s + 8;

		sprintf(buf, "0x%08x:  ", s);
		shell_print(buf);
		for(cs = s; cs < ce; ++cs)
		{
			if(cs < start || cs >= end)
			{
				shell_print("   ");
			}
			else
			{
				sprintf(buf, "%02x ", peek(cs));
				shell_print(buf);
			}
		}

		shell_print(" [ ");
		for(cs = s; cs < ce; ++cs)
		{
			if(cs < start || cs >= end)
			{
				shell_char(' ');
			}
			else
			{
				int val = peek(cs);
				sprintf(buf, "%c", isprint(val) ? val : '.');
				shell_print(buf);
			}
		}

		shell_print(" ]\n");
	}
}

static i32r _hexdump(i32r a, i32 *p)
{
	hexdump(p[0], p[1]);
	return 0;
	(void)a;
}

static i32r _ldr(i32r a, i32 *p)
{
	return *(u32 *)p[0];
	(void)a;
}

static i32r _ldrh(i32r a, i32 *p)
{
	return *(u16 *)p[0];
	(void)a;
}

static i32r _ldrsh(i32r a, i32 *p)
{
	return *(i16 *)p[0];
	(void)a;
}

static i32r _ldrb(i32r a, i32 *p)
{
	return *(u8 *)p[0];
	(void)a;
}

static i32r _ldrsb(i32r a, i32 *p)
{
	return *(i8 *)p[0];
	(void)a;
}

static i32r _str(i32r a, i32 *p)
{
	*(u32 *)p[0] = p[1];
	return p[1];
	(void)a;
}

static i32r _strh(i32r a, i32 *p)
{
	*(u16 *)p[0] = p[1];
	return p[1] & 0xFFFF;
	(void)a;
}

static i32r _strb(i32r a, i32 *p)
{
	*(u8 *)p[0] = p[1];
	return p[1] & 0xFF;
	(void)a;
}

static i32r _boolstr(i32r a, i32 *p)
{
	return (i32r)(p[0] ? "true" : "false");
	(void)a;
}

static i32r _fopen(i32r a, i32 *p)
{
	return f_open((FIL *)p[0], (char *)p[1], p[2]);
	(void)a;
}

static i32r _fclose(i32r a, i32 *p)
{
	return f_close((FIL *)p[0]);
	(void)a;
}

static i32r _fread(i32r a, i32 *p)
{
	return f_read((FIL *)p[0], (void *)p[1], p[2], (UINT *)p[3]);
	(void)a;
}

static i32r _fwrite(i32r a, i32 *p)
{
	return f_write((FIL *)p[0], (void *)p[1], p[2], (UINT *)p[3]);
	(void)a;
}

static i32r _fseek(i32r a, i32 *p)
{
	return f_lseek((FIL *)p[0], p[1]);
	(void)a;
}

static i32r _fsync(i32r a, i32 *p)
{
	return f_sync((FIL *)p[0]);
	(void)a;
}

static i32r _ftruncate(i32r a, i32 *p)
{
	return f_truncate((FIL *)p[0]);
	(void)a;
}

static i32r _opendir(i32r a, i32 *p)
{
	return f_opendir((DIR *)p[0], (char *)p[1]);
	(void)a;
}

static i32r _closedir(i32r a, i32 *p)
{
	return f_closedir((DIR *)p[0]);
	(void)a;
}

static i32r _readdir(i32r a, i32 *p)
{
	return f_readdir((DIR *)p[0], (FILINFO *)p[1]);
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
	{ .Name = "help",          .NumArgs = 0, .IsVariadic = 1 },

	{ .Name = "delay_ms",      .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "pin_high",      .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "pin_low",       .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "pin_toggle",    .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "pin_get",       .NumArgs = 2, .IsVariadic = 0 },

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
	{ .Name = "pow",           .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "rand",          .NumArgs = 0, .IsVariadic = 0 },
	{ .Name = "srand",         .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "blkinfo",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "mkfs",          .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "mount",         .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "unmount",       .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "mkdir",         .NumArgs = 1, .IsVariadic = 1 },
	{ .Name = "create",        .NumArgs = 1, .IsVariadic = 1 },
	{ .Name = "rm",            .NumArgs = 1, .IsVariadic = 1 },
	{ .Name = "mv",            .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "cp",            .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "ls",            .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "edit",          .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "edsav",         .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "fserr",         .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "memcpy",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memset",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memcmp",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memmove",       .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "memchr",        .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "strlen",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "strnlen",       .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strcpy",        .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strncpy",       .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "strcmp",        .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strncmp",       .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "strchr",        .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "clipget",       .NumArgs = 0, .IsVariadic = 0 },
	{ .Name = "clipsave",      .NumArgs = 1, .IsVariadic = 1 },

	{ .Name = "hexdump",       .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "ldr",           .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "ldrh",          .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "ldrsh",         .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "ldrb",          .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "ldrsb",         .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "str",           .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strh",          .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "strb",          .NumArgs = 2, .IsVariadic = 0 },

	{ .Name = "boolstr",       .NumArgs = 1, .IsVariadic = 0 },

	{ .Name = "fopen",         .NumArgs = 3, .IsVariadic = 0 },
	{ .Name = "fclose",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "fread",         .NumArgs = 4, .IsVariadic = 0 },
	{ .Name = "fwrite",        .NumArgs = 4, .IsVariadic = 0 },
	{ .Name = "fseek",         .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "fsync",         .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "ftrunc",        .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "opendir",       .NumArgs = 2, .IsVariadic = 0 },
	{ .Name = "closedir",      .NumArgs = 1, .IsVariadic = 0 },
	{ .Name = "readdir",       .NumArgs = 2, .IsVariadic = 0 },
};

static const NanoC_ParserBuiltins parser_builtins =
{
	.Count = ARRLEN(parser_builtins_data),
	.Table = parser_builtins_data
};

#define HELP_COLS      4
#define HELP_COLWIDTH 15
#define HELP_HEIGHT   19

static i32r _help(i32r a, i32 *p)
{
	int n = 0, i = 0, end;
	char buf[16];
	const NanoC_ParserBuiltin *builtin = parser_builtins_data;

	if(a > 1)
	{
		shell_print("Invalid arguments\n");
		return 1;
	}

	if(a == 1)
	{
		i = p[0];
		builtin += i;
	}

	end = i + HELP_COLS * HELP_HEIGHT;
	if(end > (int)ARRLEN(parser_builtins_data))
	{
		end = (int)ARRLEN(parser_builtins_data);
	}

	for(; i < end; ++i, ++builtin, ++n)
	{
		buf[0] = builtin->IsVariadic ? '>' : ' ';
		buf[1] = builtin->NumArgs + '0';
		buf[2] = ' ';
		strcpy(buf + 3, builtin->Name);
		shell_xy((n % HELP_COLS) * HELP_COLWIDTH, n / HELP_COLS + 1);
		shell_print(buf);
	}

	return 0;
}

static const NanoC_Builtin functions[] =
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
	_pin_get,

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
	_pow,

	_rand,
	_srand,

	_blkinfo,
	_mkfs,
	_mount,
	_unmount,
	_mkdir,
	_create,
	_rm,
	_mv,
	_cp,
	_ls,
	_edit,
	_edsav,
	_fserr,

	_memcpy,
	_memset,
	_memcmp,
	_memmove,
	_memchr,
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

	_ldr,
	_ldrh,
	_ldrsh,
	_ldrb,
	_ldrsb,
	_str,
	_strh,
	_strb,

	_boolstr,

	_fopen,
	_fclose,
	_fread,
	_fwrite,
	_fseek,
	_fsync,
	_ftruncate,
	_opendir,
	_closedir,
	_readdir,
};

static const NanoC_Builtins builtins = { ARRLEN(functions), functions };

#define ROMSTART 0x08000000
#define ROMEND   (ROMSTART + 2048 * 1024)

#define RAMSTART 0x20000000
#define RAMEND   (RAMSTART + 256 * 1024)

static const char *addrmem(size_t addr)
{
	if(addr >= ROMSTART && addr < ROMEND)
	{
		return "ROM";
	}

	if(addr >= RAMSTART && addr < RAMEND)
	{
		return "RAM";
	}

	return NULL;
}

#define MAX_NT_SEARCH 128

static int isstring(const char *ptr)
{
	const char *nt = memchr(ptr, 0, MAX_NT_SEARCH);
	if(nt == NULL)
	{
		return 0;
	}

	while(ptr < nt)
	{
		if(!isprint(*ptr++))
		{
			return 0;
		}
	}

	return 1;
}

static void printaddr(size_t addr)
{
	const char *mem = addrmem(addr);
	const char *ptr = (const char *)addr;
	if(!mem)
	{
		return;
	}

	if(!isstring(ptr))
	{
		return;
	}

	shell_char('[');
	shell_print(mem);
	shell_print("]: \"");
	shell_print(ptr);
	shell_print("\"\n");
}

static void compile(const char *src, int length)
{
	NanoC_Status status;
	NanoC_Parser parser;
	u8 output_buf[256];
	char strings[256];
	char buf[128];
	size_t rv = 0;

	nanoc_parser_init(&parser, src, strings, output_buf,
		sizeof(output_buf), &parser_builtins);
	status = nanoc_statement(&parser);
	if(status)
	{
		sprintf(buf, "\nParse error: %s\n", nanoc_status_message(status));
		shell_print(buf);
		return;
	}

	nanoc_output_emit(&parser.Output, NANOC_INSTR_HALT);

	status = nanoc_interpreter_run(parser.Output.Buffer, &builtins, &rv);
	if(status)
	{
		sprintf(buf, "\nRuntime error: %s\n",
			nanoc_status_message(status));

		shell_print(buf);
		return;
	}

	sprintf(buf, "\n\nREADY (%d / 0x%x)\n", rv, rv);
	shell_print(buf);
	printaddr(rv);
	(void)length;
}

void command_run(const char *cmd, int len)
{
	compile(cmd, len);
}
