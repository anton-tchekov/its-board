/**
 * @file    nanoc_tokenstream.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC token stream
 */

#ifndef __NANOC_TOKENSTREAM_H__
#define __NANOC_TOKENSTREAM_H__

#include "nanoc_types.h"
#include "nanoc_lexer.h"

#define NANOC_TOKEN_BUFFER_SIZE 2

typedef struct NANOC_TOKEN_STREAM
{
	NanoC_Lexer Lexer;
	NanoC_Token Buffer[NANOC_TOKEN_BUFFER_SIZE];
	u8 ReadIndex;
} NanoC_TokenStream;

/**
 * @brief Initialize the Token Stream and the Lexer
 * @param ts Token Stream
 * @param source Source code
 */
void nanoc_tokenstream_init(NanoC_TokenStream *ts, const char *source,
	char *strings);

/**
 * @brief Get a token from the buffer
 * @param ts Token Stream
 * @param n Number of tokens to look ahead, 0 is the current token
 *          (must be less than NANOC_TOKEN_BUFFER_SIZE)
 */
NanoC_Token *nanoc_tokenstream_get(NanoC_TokenStream *ts, u8r n);

/**
 * @brief Get the next token from the lexer and put it into the buffer
 * @param ts Token Stream
 */
void nanoc_tokenstream_next(NanoC_TokenStream *ts);

#endif /* __NANOC_TOKENSTREAM_H__ */
