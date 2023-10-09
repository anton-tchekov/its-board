/**
 * @file    nanoc_lexer_identifier.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC lexer identifier
 */

#ifndef __NANOC_LEXER_IDENTIFIER_H__
#define __NANOC_LEXER_IDENTIFIER_H__

#include "nanoc_lexer.h"

NanoC_TokenType nanoc_keyword_detect(const char *ident, size_t len);
NanoC_Status nanoc_lexer_identifier(NanoC_Lexer *lexer, NanoC_Token *token);

#endif /* __NANOC_LEXER_IDENTIFIER_H__ */
