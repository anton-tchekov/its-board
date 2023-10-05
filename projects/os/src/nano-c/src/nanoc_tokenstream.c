#include "nanoc_tokenstream.h"
#include "nanoc_debug.h"

static inline u8r wrap_size(u8r idx)
{
	return idx % NANOC_TOKEN_BUFFER_SIZE;
}

void nanoc_tokenstream_init(NanoC_TokenStream *ts, const char *source,
	char *strings)
{
	u8r i;
	nanoc_lexer_init(&ts->Lexer, source, strings);
	ts->ReadIndex = 0;
	for(i = 0; i < NANOC_TOKEN_BUFFER_SIZE; ++i)
	{
		nanoc_lexer_next(&ts->Lexer, &ts->Buffer[i]);
	}
}

NanoC_Token *nanoc_tokenstream_get(NanoC_TokenStream *ts, u8r n)
{
	nanoc_assert(n < NANOC_TOKEN_BUFFER_SIZE);
	return &ts->Buffer[wrap_size(ts->ReadIndex + n)];
}

void nanoc_tokenstream_next(NanoC_TokenStream *ts)
{
	nanoc_lexer_next(&ts->Lexer, &ts->Buffer[ts->ReadIndex]);
	ts->ReadIndex = wrap_size(ts->ReadIndex + 1);
}
