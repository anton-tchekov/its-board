/**
 * @file    nanoc_if.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_if.h"
#include "nanoc_status.h"
#include "nanoc_expression.h"
#include "nanoc_instruction.h"

NanoC_Status nanoc_if(NanoC_Parser *parser)
{
	NanoC_Address addr;
	NEXT();
	EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NEXT();
	NANOC_PROPAGATE(nanoc_expression(parser));
	EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	addr = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JZ);
	NEXT();
	NANOC_PROPAGATE(nanoc_block(parser));
	nanoc_output_jump_here(&parser->Output, addr);
	if(TT(1) == NANOC_TT_ELSE)
	{
		addr = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JMP);
		NEXT();
		if(TT(1) == NANOC_TT_IF)
		{
			NEXT();
			NANOC_PROPAGATE(nanoc_if(parser));
		}
		else
		{
			NEXT();
			NANOC_PROPAGATE(nanoc_block(parser));
		}

		nanoc_output_jump_here(&parser->Output, addr);
	}

	return NANOC_STATUS_SUCCESS;
}
