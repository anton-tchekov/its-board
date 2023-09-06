#ifndef __NANOC_LOOP_H__
#define __NANOC_LOOP_H__

#include "nanoc_parser.h"

NanoC_Status nanoc_loop(NanoC_Parser *parser);
NanoC_Status nanoc_while(NanoC_Parser *parser);
NanoC_Status nanoc_do_while(NanoC_Parser *parser);
NanoC_Status nanoc_for(NanoC_Parser *parser);
NanoC_Status nanoc_break(NanoC_Parser *parser);
NanoC_Status nanoc_continue(NanoC_Parser *parser);

#endif /* __NANOC_LOOP_H__ */
