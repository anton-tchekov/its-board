#include "nanoc_if.h"
#include "nanoc_status.h"
#include "nanoc_expression.h"
#include "nanoc_instruction.h"

u8r nanoc_if(NanoC_Parser *parser)
{
	u16r addr;
	NEXT();
	EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NEXT();
	PROPAGATE(nanoc_expression(parser));
	EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	addr = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JZ);
	NEXT();
	PROPAGATE(nanoc_block(parser));
	nanoc_output_jump_here(&parser->Output, addr);
	if(TT(1) == NANOC_TT_ELSE)
	{
		addr = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JMP);
		NEXT();
		if(TT(1) == NANOC_TT_IF)
		{
			NEXT();
			PROPAGATE(nanoc_if(parser));
		}
		else
		{
			NEXT();
			PROPAGATE(nanoc_block(parser));
		}

		nanoc_output_jump_here(&parser->Output, addr);
	}

	return NANOC_STATUS_SUCCESS;
}
