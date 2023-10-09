/**
 * @file    nanoc_if.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC if-else parser
 */

#ifndef __NANOC_IF_H__
#define __NANOC_IF_H__

#include "nanoc_types.h"
#include "nanoc_parser.h"

/**
 * @brief Parse if, else-if, else statement
 *
 * @param parser Parser
 * @return Status code
 */
NanoC_Status nanoc_if(NanoC_Parser *parser);

#endif /* __NANOC_IF_H__ */
