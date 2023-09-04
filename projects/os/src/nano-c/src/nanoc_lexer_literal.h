#ifndef __NANOC_LEXER_LITERAL_H__
#define __NANOC_LEXER_LITERAL_H__

#include "types.h"
#include "nanoc_lexer.h"

u8r nanoc_lexer_char(NanoC_Lexer *lexer, NanoC_Token *token);
u8r nanoc_lexer_string(NanoC_Lexer *lexer, NanoC_Token *token);

#endif /* __NANOC_LEXER_LITERAL_H__ */
