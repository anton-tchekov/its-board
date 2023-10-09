/**
 * @file    nanoc_lexer_literal.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC lexer literal
 */

#ifndef __NANOC_LEXER_LITERAL_H__
#define __NANOC_LEXER_LITERAL_H__

#include "nanoc_lexer.h"

/**
 * @brief Try to tokenize a character
 *
 * @param lexer Lexer
 * @param token Token output
 * @return true if a character was detected, otherwise false
 */
NanoC_Bool nanoc_lexer_char(NanoC_Lexer *lexer, NanoC_Token *token);

/**
 * @brief Try to tokenize a string
 *
 * @param lexer Lexer
 * @param token Token output
 * @return true if a string was detected, otherwise false
 */
NanoC_Bool nanoc_lexer_string(NanoC_Lexer *lexer, NanoC_Token *token);

#endif /* __NANOC_LEXER_LITERAL_H__ */
