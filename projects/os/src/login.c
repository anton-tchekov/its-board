#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "terminal.h"
#include "mode.h"

#define OFFSET_X 10
#define OFFSET_Y  1

enum
{
	LOGIN_STATE_ERROR,
	LOGIN_STATE_INPUT,
};

/* 32 bytes */
typedef struct
{
	char Input[30];
	uint8_t Length;
	uint8_t State;
} LoginManager;

static LoginManager _lm;
static const char *_password = "geheim123!";

static void println(int x, int y, int style, const char *s)
{
	int c;
	while((c = *s++))
	{
		terminal_set(x++, y, c | style);
	}

	for(; x < TERMINAL_W; ++x)
	{
		terminal_set(x, y, ' ' | style);
	}
}

void login_init(void)
{
	_lm.State = LOGIN_STATE_INPUT;
	_lm.Length = 0;
	println(0, OFFSET_Y, TERMINAL_FG_WHITE | TERMINAL_BG_BLACK,
		"Password:");
	terminal_set(OFFSET_X, OFFSET_Y,
		' ' | TERMINAL_BG_WHITE | TERMINAL_FG_BLACK);
	println(0, OFFSET_Y + 1, TERMINAL_FG_WHITE | TERMINAL_BG_BLACK,
		"");
}

static void login_char(LoginManager *lm, int c)
{
	if(lm->Length >= sizeof(lm->Input))
	{
		return;
	}

	lm->Input[lm->Length] = c;
	terminal_set(OFFSET_X + lm->Length, OFFSET_Y,
		'*' | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
	++lm->Length;
	terminal_set(OFFSET_X + lm->Length, OFFSET_Y,
		' ' | TERMINAL_FG_BLACK | TERMINAL_BG_WHITE);
}

static void login_backspace(LoginManager *lm)
{
	if(lm->Length == 0)
	{
		return;
	}

	terminal_set(OFFSET_X + lm->Length, OFFSET_Y,
		' ' | TERMINAL_BG_BLACK | TERMINAL_FG_WHITE);
	--lm->Length;
	terminal_set(OFFSET_X + lm->Length, OFFSET_Y,
		' ' | TERMINAL_FG_BLACK | TERMINAL_BG_WHITE);
}

static void login_enter(LoginManager *lm)
{
	if(lm->Length == 0)
	{
		return;
	}

	if(!strncmp(lm->Input, _password, lm->Length))
	{
		mode_set(MODE_SHELL);
	}
	else
	{
		lm->State = LOGIN_STATE_ERROR;
		println(0, OFFSET_Y, TERMINAL_FG_RED | TERMINAL_BG_BLACK,
			"[ Wrong Password ]");

		println(0, OFFSET_Y + 1, TERMINAL_FG_WHITE | TERMINAL_BG_BLACK,
			"Press any key to retry");
	}
}

void login_key(int key, int c)
{
	if(_lm.State == LOGIN_STATE_INPUT)
	{
		if(c == '\b')
		{
			login_backspace(&_lm);
		}
		else if(c == '\n')
		{
			login_enter(&_lm);
		}
		else if(isprint(c))
		{
			login_char(&_lm, c);
		}
	}
	else if(_lm.State == LOGIN_STATE_ERROR)
	{
		login_init();
	}
}
