#include "nanoc_data.h"
#if 0

static u8r nanoc_data_identifier(NanoC_Parser *parser)
{
	u8r tt;
	NEXT();
	tt = TT(0);
	if(tt == NANOC_TT_EQU)
	{

	}
	else if(tt == NANOC_TT_COLON)
	{

	}
}

u8r nanoc_data(NanoC_Parser *parser)
{
	switch(TT(0))
	{
	case NANOC_TT_IDENTIFIER: return nanoc_data_identifier(parser);
	case NANOC_TT_DCB:        return nanoc_data_dcb(parser);
	case NANOC_TT_DCW:        return nanoc_data_dcw(parser);
	case NANOC_TT_DCD:        return nanoc_data_dcd(parser);
	case NANOC_TT_FILL:       return nanoc_data_fill(parser);
	}

	return NANOC_ERROR_UNEXPECTED_TOKEN;
}

#endif
