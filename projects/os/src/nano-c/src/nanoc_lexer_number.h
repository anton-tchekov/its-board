/**
 * @file    nanoc_lexer_number.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC lexer number
 */

#ifndef __NANOC_LEXER_NUMBER_H__
#define __NANOC_LEXER_NUMBER_H__

#include "nanoc_types.h"
#include "nanoc_lexer.h"

/**
 * @brief Try to tokenize a number
 *
 * @param lexer Lexer
 * @param token Token output
 * @return true if a number was detected, otherwise false
 */
NanoC_Bool nanoc_lexer_number(NanoC_Lexer *lexer, NanoC_Token *token);

#endif /* __NANOC_LEXER_NUMBER_H__ */
