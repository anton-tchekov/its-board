/**
 * @file    overlay.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "overlay.h"
#include "keyboard.h"
#include "types.h"
#include "mode.h"

#include <stdarg.h>

enum
{
	OVERLAY_ALERT,
	OVERLAY_CONFIRM,
	OVERLAY_PROMPT,
};

typedef struct
{
	union
	{
		void (*Alert)(void);
		void (*Confirm)(int);
		void (*Prompt)(int, char *);
	} Callback;
	u8 Element;
	u8 Type;
} Overlay;

Overlay _overlay;

static size_t lines_count(const char *str)
{
	int c;
	size_t n = 1;
	while((c = *str++))
	{
		if(c == '\n')
		{
			++n;
		}
	}

	return n;
}

static size_t longest_line(const char *str)
{
	int c;
	size_t cnt = 0;
	size_t max = 0;
	while((c = *str++))
	{
		if(c == '\n')
		{
			max = (cnt > max) ? cnt : max;
			cnt = 0;
		}
	}

	return max;
}

static void overlay_open(void)
{
	mode_set(MODE_OVERLAY);
}

void alert(int type, void (*callback)(void),
	const char *msg, ...)
{
	_overlay.Type = OVERLAY_ALERT;
	overlay_open();
}

void confirm(int type, void (*callback)(int),
	const char *msg, ...)
{
	_overlay.Type = OVERLAY_CONFIRM;
	_overlay.Callback.Confirm = callback;
	overlay_open();
}

void prompt(int type, void (*callback)(int, char *),
	const char *def, const char *msg, ...)
{
	_overlay.Type = OVERLAY_PROMPT;
	_overlay.Callback.Prompt = callback;
	overlay_open();
}

static void alert_key(int key)
{
	if(key == KEY_RETURN)
	{
		_overlay.Callback.Alert();
	}
}

static void confirm_key(int key)
{
	switch(key)
	{
	case KEY_ESCAPE:
		_overlay.Callback.Confirm(0);
		break;

	case KEY_RETURN:
		_overlay.Callback.Confirm(_overlay.Element);
		break;

	case MOD_SHIFT | KEY_TAB:
	case KEY_TAB:
	case KEY_UP:
	case KEY_LEFT:
	case KEY_DOWN:
	case KEY_RIGHT:
		_overlay.Element = !_overlay.Element;
		break;
	}
}

static void prompt_key(int key, int c)
{
	switch(key)
	{
	case MOD_SHIFT | KEY_TAB:
	case KEY_UP:
		_overlay.Element = (_overlay.Element == 0) ?
			(2) : (_overlay.Element - 1);
		break;

	case KEY_TAB:
	case KEY_DOWN:
		_overlay.Element = (_overlay.Element == 2) ?
			(0) : (_overlay.Element + 1);
		break;

	default:
		break;
	}
}

void overlay_key(int key, int c)
{
	switch(_overlay.Type)
	{
	case OVERLAY_ALERT:
		alert_key(key);
		break;

	case OVERLAY_CONFIRM:
		confirm_key(key);
		break;

	case OVERLAY_PROMPT:
		prompt_key(key, c);
		break;
	}
}
