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
	if(len > CLIPBOARD_MAX_LENGTH)
	{
		return 0;
	}

	_clipboard_len = len;
	memcpy(_clipboard_text, text, len);
	return 1;
}
