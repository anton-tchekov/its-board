/**
 * @file    nanoc_tokentype.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_tokentype.h"
#include "nanoc_debug.h"


const char *nanoc_tokentype_tostring(NanoC_TokenType token_type)
{
	const char *token_names[] =
	{
		[ NANOC_TT_NULL ]           = "TT_NULL",
		[ NANOC_TT_UNKNOWN ]        = "TT_UNKNOWN",

		[ NANOC_TT_IDENTIFIER ]     = "TT_IDENTIFIER",
		[ NANOC_TT_INTEGER ]        = "TT_INTEGER",
		[ NANOC_TT_FLOAT ]          = "TT_FLOAT",
		[ NANOC_TT_STRING ]         = "TT_STRING",

		[ NANOC_TT_FN ]             = "TT_FN",
		[ NANOC_TT_IF ]             = "TT_IF",
		[ NANOC_TT_ELSE ]           = "TT_ELSE",
		[ NANOC_TT_DO ]             = "TT_DO",
		[ NANOC_TT_WHILE ]          = "TT_WHILE",
		[ NANOC_TT_FOR ]            = "TT_FOR",
		[ NANOC_TT_LOOP ]           = "TT_LOOP",
		[ NANOC_TT_INT ]            = "TT_INT",
		[ NANOC_TT_BREAK ]          = "TT_BREAK",
		[ NANOC_TT_CONTINUE ]       = "TT_CONTINUE",
		[ NANOC_TT_RETURN ]         = "TT_RETURN",

		[ NANOC_TT_L_PAREN ]        = "TT_L_PAREN",
		[ NANOC_TT_R_PAREN ]        = "TT_R_PAREN",
		[ NANOC_TT_L_BRACKET ]      = "TT_L_BRACKET",
		[ NANOC_TT_R_BRACKET ]      = "TT_R_BRACKET",
		[ NANOC_TT_L_BRACE ]        = "TT_L_BRACE",
		[ NANOC_TT_R_BRACE ]        = "TT_L_BRACE",
		[ NANOC_TT_DOT ]            = "TT_DOT",
		[ NANOC_TT_COMMA ]          = "TT_COMMA",
		[ NANOC_TT_COLON ]          = "TT_COLON",
		[ NANOC_TT_SEMICOLON ]      = "TT_SEMICOLON",
		[ NANOC_TT_BACKSLASH ]      = "TT_BACKSLASH",
		[ NANOC_TT_ASSIGN ]         = "TT_ASSIGN",
		[ NANOC_TT_ADD_ASSIGN ]     = "TT_ADD_ASSIGN",
		[ NANOC_TT_SUB_ASSIGN ]     = "TT_SUB_ASSIGN",
		[ NANOC_TT_MUL_ASSIGN ]     = "TT_MUL_ASSIGN",
		[ NANOC_TT_DIV_ASSIGN ]     = "TT_DIV_ASSIGN",
		[ NANOC_TT_MOD_ASSIGN ]     = "TT_MOD_ASSIGN",
		[ NANOC_TT_SHL_ASSIGN ]     = "TT_SHL_ASSIGN",
		[ NANOC_TT_SHR_ASSIGN ]     = "TT_SHR_ASSIGN",
		[ NANOC_TT_B_OR_ASSIGN ]    = "TT_B_OR_ASSIGN",
		[ NANOC_TT_B_AND_ASSIGN ]   = "TT_B_AND_ASSIGN",
		[ NANOC_TT_B_NOT_ASSIGN ]   = "TT_B_NOT_ASSIGN",
		[ NANOC_TT_XOR_ASSIGN ]     = "TT_XOR_ASSIGN",
		[ NANOC_TT_INCREMENT ]      = "TT_INCREMENT",
		[ NANOC_TT_DECREMENT ]      = "TT_DECREMENT",
		[ NANOC_TT_ARROW ]          = "TT_ARROW",

		[ NANOC_TT_L_OR ]           = "TT_L_OR",
		[ NANOC_TT_L_AND ]          = "TT_L_AND",
		[ NANOC_TT_B_OR ]           = "TT_B_OR",
		[ NANOC_TT_B_XOR ]          = "TT_B_XOR",
		[ NANOC_TT_B_AND ]          = "TT_B_AND",
		[ NANOC_TT_EQ ]             = "TT_EQ",
		[ NANOC_TT_NE ]             = "TT_NE",
		[ NANOC_TT_LT ]             = "TT_LT",
		[ NANOC_TT_GT ]             = "TT_GT",
		[ NANOC_TT_LE ]             = "TT_LE",
		[ NANOC_TT_GE ]             = "TT_GE",
		[ NANOC_TT_SHL ]            = "TT_SHL",
		[ NANOC_TT_SHR ]            = "TT_SHR",
		[ NANOC_TT_ADD ]            = "TT_ADD",
		[ NANOC_TT_SUB ]            = "TT_SUB",
		[ NANOC_TT_MUL ]            = "TT_MUL",
		[ NANOC_TT_DIV ]            = "TT_DIV",
		[ NANOC_TT_MOD ]            = "TT_MOD",
		[ NANOC_TT_L_NOT ]          = "TT_L_NOT",
		[ NANOC_TT_B_NOT ]          = "TT_B_NOT",
		[ NANOC_TT_U_MINUS ]        = "TT_U_MINUS",
	};

	return token_names[token_type];
}
#ifdef NANOC_DEBUG

#endif /* NANOC_DEBUG */
