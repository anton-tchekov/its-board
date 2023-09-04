#include "nanoc_loop.h"
#include "nanoc_status.h"
#include "nanoc_instruction.h"
#include "nanoc_expression.h"

static void address_stack_update(
	NanoC_Output *output, NanoC_AddressStack *as, u8r prev, u16r addr)
{
	u8r top = as->Top;
	u16 *stack = as->Stack;
	while(top > prev)
	{
		--top;
		nanoc_output_emit16_at(output, stack[top], addr);
	}

	as->Top = top;
}

static u8r loop_nest(NanoC_Parser *parser)
{
	++parser->BreakNesting;
	++parser->ContinueNesting;
	PROPAGATE(nanoc_block(parser));
	--parser->BreakNesting;
	--parser->ContinueNesting;
	return NANOC_STATUS_SUCCESS;
}

static void handle_break_continue(NanoC_Parser *parser,
	u8r prev_break, u8r prev_continue, u8r idx_branch)
{
	address_stack_update(&parser->Output, &parser->BreakStack,
		prev_break, parser->Output.Pos);

	address_stack_update(&parser->Output, &parser->ContinueStack,
		prev_continue, idx_branch);
}

static void break_continue(NanoC_Parser *parser, NanoC_AddressStack *stack)
{
	/*address_stack_push(stack,
		nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JMP));

	nanoc_output_emit16_at(&parser->Output, as->Stack[as->Top], addr);
	++as->Top;*/
}

u8r nanoc_loop(NanoC_Parser *parser)
{
	u8r prev_break = parser->BreakStack.Top;
	u8r prev_continue = parser->ContinueStack.Top;
	u16r idx_before = parser->Output.Pos;
	NEXT();
	PROPAGATE(loop_nest(parser));
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JMP, idx_before);
	handle_break_continue(parser, prev_break, prev_continue, idx_before);
	return NANOC_STATUS_SUCCESS;
}

#if 0
u8r nanoc_for(NanoC_Parser *parser)
{
	NEXT();
	EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NEXT();
	PROPAGATE(nanoc_statement(parser));
	NEXT();
	PROPAGATE(nanoc_expression(parser));

	NEXT();
	PROPAGATE(nanoc_statement(parser));

	PROPAGATE(loop_nest(parser));


	return NANOC_STATUS_SUCCESS;
}
#endif

u8r nanoc_while(NanoC_Parser *parser)
{
	u16r idx_branch;
	u16r idx_before = parser->Output.Pos;
	u8r prev_break = parser->BreakStack.Top;
	u8r prev_continue = parser->ContinueStack.Top;
	NEXT();
	EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NEXT();
	PROPAGATE(nanoc_expression(parser));
	EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	idx_branch = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JZ);
	NEXT();
	PROPAGATE(loop_nest(parser));
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JMP, idx_before);
	nanoc_output_emit16_at(&parser->Output, idx_branch, parser->Output.Pos);
	handle_break_continue(parser, prev_break, prev_continue, idx_before);
	return NANOC_STATUS_SUCCESS;
}

u8r nanoc_do_while(NanoC_Parser *parser)
{
	u16r idx_branch;
	u16r idx_begin = parser->Output.Pos;
	u8r prev_break = parser->BreakStack.Top;
	u8r prev_continue = parser->ContinueStack.Top;
	NEXT();
	PROPAGATE(loop_nest(parser));
	NEXT();
	EXPECT(NANOC_TT_WHILE, NANOC_ERROR_EXPECTED_WHILE);
	NEXT();
	EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NEXT();
	PROPAGATE(nanoc_expression(parser));
	EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	NEXT();
	EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	idx_branch = parser->Output.Pos;
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JNZ, idx_begin);
	handle_break_continue(parser, prev_break, prev_continue, idx_branch);
	return NANOC_STATUS_SUCCESS;
}

u8r nanoc_break(NanoC_Parser *parser)
{
	NEXT();
	EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	if(!parser->BreakNesting)
	{
		THROW(NANOC_ERROR_BREAK_NOT_WITHIN_LOOP_OR_SWITCH);
	}

	break_continue(parser, &parser->BreakStack);
	return NANOC_STATUS_SUCCESS;
}

u8r nanoc_continue(NanoC_Parser *parser)
{
	NEXT();
	EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	if(!parser->ContinueNesting)
	{
		THROW(NANOC_ERROR_CONTINUE_NOT_WITHIN_LOOP);
	}

	break_continue(parser, &parser->ContinueStack);
	return NANOC_STATUS_SUCCESS;
}
