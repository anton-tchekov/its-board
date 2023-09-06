#ifndef __NANOC_LOOP_H__
#define __NANOC_LOOP_H__

#include "nanoc_parser.h"

u8r nanoc_loop(NanoC_Parser *parser);
u8r nanoc_while(NanoC_Parser *parser);
u8r nanoc_do_while(NanoC_Parser *parser);
u8r nanoc_for(NanoC_Parser *parser);
u8r nanoc_break(NanoC_Parser *parser);
u8r nanoc_continue(NanoC_Parser *parser);

#endif /* __NANOC_LOOP_H__ */
