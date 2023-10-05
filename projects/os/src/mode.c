/**
 * @file    mode.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "mode.h"
#include "types.h"

#include "login.h"
#include "shell.h"
#include "editor.h"
#include "manager.h"
#include "test.h"
#include "overlay.h"

static u8 _mode;

void mode_set(int mode)
{
	if(_mode == MODE_LOGIN || _mode == MODE_OVERLAY)
	{
		return;
	}

	_mode = mode;
	switch(_mode)
	{
	case MODE_LOGIN:
		login_open();
		break;

	case MODE_SHELL:
		shell_open();
		break;

	case MODE_EDITOR:
		editor_open();
		break;

	case MODE_MANAGER:
		manager_open();
		break;

	case MODE_TEST:
		test_open();
		break;
	}
}

void mode_unlock(void)
{
	_mode = MODE_SHELL;
	shell_open();
}

void mode_key(int key, int c)
{
	switch(_mode)
	{
	case MODE_LOGIN:
		login_key(key, c);
		break;

	case MODE_SHELL:
		shell_key(key, c);
		break;

	case MODE_EDITOR:
		editor_key(key, c);
		break;

	case MODE_MANAGER:
		manager_key(key, c);
		break;

	case MODE_TEST:
		test_key(key, c);
		break;

	case MODE_OVERLAY:
		overlay_key(key, c);
		break;
	}
}
