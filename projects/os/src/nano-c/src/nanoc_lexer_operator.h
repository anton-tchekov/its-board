/**
 * @file    nanoc_lexer_operator.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC lexer operator
 */

#ifndef __NANOC_LEXER_OPERATOR_H__
#define __NANOC_LEXER_OPERATOR_H__

#include "types.h"
#include "nanoc_lexer.h"

/**
 * @brief Tokenize an operator. If no operator is detected,
 *        a token of type `unknown` is generated.
 *
 * @param lexer Lexer
 * @param token Token output
 */
void nanoc_lexer_operator(NanoC_Lexer *lexer, NanoC_Token *token);

#endif /* __NANOC_LEXER_OPERATOR_H__ */
