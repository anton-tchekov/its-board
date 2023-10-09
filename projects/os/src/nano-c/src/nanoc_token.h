/**
 * @file    nanoc_token.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC token
 */

#ifndef __NANOC_TOKEN_H__
#define __NANOC_TOKEN_H__

#include "types.h"
#include "nanoc_tokentype.h"

typedef struct NanoC_Position
{
	i16 Col;
	i16 Row;
} NanoC_Position;

typedef struct NANOC_TOKEN
{
	const char *Ptr;
	const char *LineBegin;
	size_t Length;
	NanoC_Value Value;
	NanoC_Position Pos;
	NanoC_TokenType Type;
} NanoC_Token;

void nanoc_token_print(NanoC_Token *token);

#endif /* __NANOC_TOKEN_H__ */
