#include "clipboard.h"
#include <string.h>

static int _clipboard_len;
static char _clipboard_text[CLIPBOARD_MAX_LENGTH];

const char *clipboard_get(int *len)
{
	*len = _clipboard_len;
	return _clipboard_text;
}

int clipboard_save(const char *text, int len)
{
	int success = 0;
	if(len <= CLIPBOARD_MAX_LENGTH)
	{
		_clipboard_len = len;
		memcpy(_clipboard_text, text, len);
		success = 1;
	}

	return success;
}
