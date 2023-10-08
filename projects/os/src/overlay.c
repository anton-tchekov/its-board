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

void alert(int type, void (*callback)(void),
	const char *msg, ...)
{
	_overlay.Callback.Alert = callback;
	mode_set(MODE_ALERT);
}

void confirm(int type, void (*callback)(int),
	const char *msg, ...)
{
	_overlay.Callback.Confirm = callback;
	mode_set(MODE_CONFIRM);
}

void prompt(int type, void (*callback)(int, char *),
	const char *def, const char *msg, ...)
{
	_overlay.Callback.Prompt = callback;
	mode_set(MODE_PROMPT);
}

void alert_key(int key, int c)
{
	if(key == KEY_RETURN)
	{
		_overlay.Callback.Alert();
	}

	(void)c;
}

void confirm_key(int key, int c)
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

	(void)c;
}

void prompt_key(int key, int c)
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
