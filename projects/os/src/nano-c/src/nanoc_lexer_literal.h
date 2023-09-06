#ifndef __NANOC_LEXER_LITERAL_H__
#define __NANOC_LEXER_LITERAL_H__

#include "nanoc_lexer.h"

NanoC_Status nanoc_lexer_char(NanoC_Lexer *lexer, NanoC_Token *token);
NanoC_Status nanoc_lexer_string(NanoC_Lexer *lexer, NanoC_Token *token);

#endif /* __NANOC_LEXER_LITERAL_H__ */
