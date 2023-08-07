/**
 * @file    german.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 * @brief   German keyboard layout conversion
 */

#include "keyboard-shared.h"

/* This is horrible, but it works */
int key_to_codepoint(int k)
{
	int nomods = k & 0xFF;

	if(nomods == KEY_TAB)                             { return '\t'; }
	else if(nomods == KEY_BACKSPACE)                  { return '\b'; }
	else if(nomods == KEY_RETURN)                     { return '\n'; }
	else if(nomods == KEY_SPACE)                      { return ' '; }
	else if(k == (KEY_COMMA | MOD_SHIFT))             { return ';'; }
	else if(k == (KEY_COMMA))                         { return ','; }
	else if(k == (KEY_PERIOD | MOD_SHIFT))            { return ':'; }
	else if(k == (KEY_PERIOD))                        { return '.'; }
	else if(k == (KEY_SLASH | MOD_SHIFT))             { return '_'; }
	else if(k == (KEY_SLASH))                         { return '-'; }
	else if(k == (KEY_BACKSLASH | MOD_SHIFT))         { return '\''; }
	else if(k == (KEY_BACKSLASH))                     { return '#'; }
	else if(k == (KEY_R_BRACKET | MOD_SHIFT))         { return '*'; }
	else if(k == (KEY_R_BRACKET | MOD_ALT_GR))        { return '~'; }
	else if(k == (KEY_R_BRACKET))                     { return '+'; }
	else if(k == (KEY_NON_US_BACKSLASH | MOD_SHIFT))  { return '>'; }
	else if(k == (KEY_NON_US_BACKSLASH | MOD_ALT_GR)) { return '|'; }
	else if(k == KEY_NON_US_BACKSLASH)                { return '<'; }
	else if(k == (KEY_MINUS | MOD_SHIFT))             { return '?'; }
	else if(k == (KEY_MINUS | MOD_ALT_GR))            { return '\\'; }
	else if(k == (KEY_EQUALS | MOD_SHIFT))            { return '`'; }
	else if(k == KEY_GRAVE)                           { return '^'; }
	else if(nomods >= KEY_A && nomods <= KEY_Z)
	{
		char c = nomods - KEY_A + 'a';

		if(c == 'z') { c = 'y'; }
		else if(c == 'y') { c = 'z'; }

		if(k & MOD_ALT_GR)
		{
			if(c == 'q') { return '@'; }
		}

		if(k & MOD_SHIFT)
		{
			if(c >= 'a' && c <= 'z')
			{
				c = c - 'a' + 'A';
			}
		}

		return c;
	}
	else if(nomods >= KEY_1 && nomods <= KEY_0)
	{
		static const char numbers[] =
			{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

		static const char numbers_shift[] =
			{ '!', '\"', 0, '$', '%', '&', '/', '(', ')', '=' };

		static const char numbers_altgr[] =
			{ 0, 0, 0, 0, 0, 0, '{', '[', ']', '}' };

		int idx = nomods - KEY_1;

		if(k & MOD_SHIFT)
		{
			return *(numbers_shift + idx);
		}
		else if(k & MOD_ALT_GR)
		{
			return *(numbers_altgr + idx);
		}
		else
		{
			return *(numbers + idx);
		}
	}

	return 0;
}
