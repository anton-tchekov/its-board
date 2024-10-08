/**
 * @file    nanoc_tokentype.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC token types
 */

#ifndef __NANOC_TOKENTYPE_H__
#define __NANOC_TOKENTYPE_H__

#include "nanoc_types.h"

typedef enum NANOC_TOKENTYPE
{
	NANOC_TT_NULL,
	NANOC_TT_UNKNOWN,

	NANOC_TT_IDENTIFIER,
	NANOC_TT_INTEGER,
	NANOC_TT_FLOAT,
	NANOC_TT_STRING,

	NANOC_TT_IF,
	NANOC_TT_ELSE,
	NANOC_TT_DO,
	NANOC_TT_WHILE,
	NANOC_TT_FOR,
	NANOC_TT_LOOP,
	NANOC_TT_BREAK,
	NANOC_TT_CONTINUE,
	NANOC_TT_RETURN,

	NANOC_TT_L_PAREN,                         /* "("   */
	NANOC_TT_R_PAREN,                         /* ")"   */
	NANOC_TT_L_BRACKET,                       /* "["   */
	NANOC_TT_R_BRACKET,                       /* "]"   */
	NANOC_TT_L_BRACE,                         /* "{"   */
	NANOC_TT_R_BRACE,                         /* "}"   */
	NANOC_TT_DOT,                             /* "."   */
	NANOC_TT_COMMA,                           /* ","   */
	NANOC_TT_COLON,                           /* ":"   */
	NANOC_TT_SEMICOLON,                       /* ";"   */
	NANOC_TT_BACKSLASH,                       /* "\"   */

	NANOC_TT_ASSIGN_START,
	NANOC_TT_ASSIGN = NANOC_TT_ASSIGN_START,  /* "="   */
	NANOC_TT_ADD_ASSIGN,                      /* "+="  */
	NANOC_TT_SUB_ASSIGN,                      /* "-="  */
	NANOC_TT_MUL_ASSIGN,                      /* "*="  */
	NANOC_TT_DIV_ASSIGN,                      /* "/="  */
	NANOC_TT_MOD_ASSIGN,                      /* "%="  */
	NANOC_TT_SHL_ASSIGN,                      /* "<<=" */
	NANOC_TT_SHR_ASSIGN,                      /* ">>=" */
	NANOC_TT_B_OR_ASSIGN,                     /* "|="  */
	NANOC_TT_B_AND_ASSIGN,                    /* "&="  */
	NANOC_TT_B_NOT_ASSIGN,                    /* "~="  */
	NANOC_TT_XOR_ASSIGN,                      /* "^="  */
	NANOC_TT_ASSIGN_END = NANOC_TT_XOR_ASSIGN,

	NANOC_TT_INCREMENT,                       /* "++"  */
	NANOC_TT_DECREMENT,                       /* "--"  */
	NANOC_TT_ARROW,                           /* "->"  */

	NANOC_TT_OP_START,
	NANOC_TT_L_OR = NANOC_TT_OP_START,        /* "||" */
	NANOC_TT_L_AND,                           /* "&&" */
	NANOC_TT_INSTR_OP_START,
	NANOC_TT_B_OR = NANOC_TT_INSTR_OP_START,  /* "|"  */
	NANOC_TT_B_XOR,                           /* "^"  */
	NANOC_TT_B_AND,                           /* "&"  */
	NANOC_TT_EQ,                              /* "==" */
	NANOC_TT_NE,                              /* "!=" */
	NANOC_TT_LT,                              /* "<"  */
	NANOC_TT_GT,                              /* ">"  */
	NANOC_TT_LE,                              /* "<=" */
	NANOC_TT_GE,                              /* ">=" */
	NANOC_TT_SHL,                             /* "<<" */
	NANOC_TT_SHR,                             /* ">>" */
	NANOC_TT_ADD,                             /* "+"  */
	NANOC_TT_SUB,                             /* "-"  */
	NANOC_TT_MUL,                             /* "*"  */
	NANOC_TT_DIV,                             /* "/"  */
	NANOC_TT_MOD,                             /* "%"  */
	NANOC_TT_L_NOT,                           /* "!"  */
	NANOC_TT_B_NOT,                           /* "~"  */
	NANOC_TT_U_MINUS,                         /* "-"  */
	NANOC_TT_OP_END = NANOC_TT_U_MINUS
} NanoC_TokenType;


const char *nanoc_tokentype_tostring(NanoC_TokenType token_type);

#ifdef NANOC_DEBUG


#endif /* NANOC_DEBUG */

#endif /* __NANOC_TOKENTYPE_H__ */
