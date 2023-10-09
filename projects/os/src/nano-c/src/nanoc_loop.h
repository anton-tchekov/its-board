/**
 * @file    nanoc_loop.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC loops: while, do-while, for, loop
 */

#ifndef __NANOC_LOOP_H__
#define __NANOC_LOOP_H__

#include "nanoc_parser.h"

/**
 * @brief Parse an (infinite) loop
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_loop(NanoC_Parser *parser);

/**
 * @brief Parse a while loop
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_while(NanoC_Parser *parser);

/**
 * @brief Parse a do-while loop
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_do_while(NanoC_Parser *parser);

/**
 * @brief Parse for loop
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_for(NanoC_Parser *parser);

/**
 * @brief Parse break statement
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_break(NanoC_Parser *parser);

/**
 * @brief Parse continue statement
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_continue(NanoC_Parser *parser);

#endif /* __NANOC_LOOP_H__ */
