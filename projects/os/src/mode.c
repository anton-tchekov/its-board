/**
 * @file    mode.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "mode.h"
#include "types.h"

#include "shell.h"
#include "editor.h"

static u8 _mode;

void mode_set(int mode)
{
	_mode = mode;
	switch(_mode)
	{
	case MODE_SHELL:
		shell_open();
		break;

	case MODE_EDITOR:
		editor_open();
		break;
	}
}

void mode_key(int key, int c)
{
	switch(_mode)
	{
	case MODE_SHELL:
		shell_key(key, c);
		break;

	case MODE_EDITOR:
		editor_key(key, c);
		break;
	}
}
