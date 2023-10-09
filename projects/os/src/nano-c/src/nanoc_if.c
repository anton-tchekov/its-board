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

	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NANOC_NEXT();
	NANOC_PROPAGATE(nanoc_expression(parser));
	NANOC_EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	addr = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JZ);
	NANOC_NEXT();
	NANOC_PROPAGATE(nanoc_block(parser));
	nanoc_output_jump_here(&parser->Output, addr);
	if(NANOC_TT(1) == NANOC_TT_ELSE)
	{
		addr = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JMP);
		NANOC_NEXT();
		if(NANOC_TT(1) == NANOC_TT_IF)
		{
			NANOC_NEXT();
			NANOC_PROPAGATE(nanoc_if(parser));
		}
		else
		{
			NANOC_NEXT();
			NANOC_PROPAGATE(nanoc_block(parser));
		}

		nanoc_output_jump_here(&parser->Output, addr);
	}

	return NANOC_STATUS_SUCCESS;
}
