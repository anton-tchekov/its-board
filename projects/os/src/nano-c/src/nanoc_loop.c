/**
 * @file    nanoc_loop.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 */

#include "nanoc_loop.h"
#include "nanoc_status.h"
#include "nanoc_instruction.h"
#include "nanoc_expression.h"
#include "nanoc_address_stack.h"

static NanoC_Status loop_nest(NanoC_Parser *parser)
{
	++parser->BreakNesting;
	++parser->ContinueNesting;
	NANOC_PROPAGATE(nanoc_block(parser));
	--parser->BreakNesting;
	--parser->ContinueNesting;
	return NANOC_STATUS_SUCCESS;
}

static void break_continue_addr(NanoC_Parser *parser,
	size_t prev_break, size_t prev_continue, NanoC_Address idx_branch)
{
	nanoc_address_stack_update(&parser->BreakStack, &parser->Output,
		prev_break, parser->Output.Pos);

	nanoc_address_stack_update(&parser->ContinueStack, &parser->Output,
		prev_continue, idx_branch);
}

static void break_continue_jump(NanoC_Output *output, NanoC_AddressStack *stack)
{
	nanoc_address_stack_push(stack,
		nanoc_output_unknown_jump(output, NANOC_INSTR_JMP));
}

NanoC_Status nanoc_loop(NanoC_Parser *parser)
{
	NanoC_Address idx_before;
	size_t prev_break, prev_continue;

	prev_break = parser->BreakStack.Top;
	prev_continue = parser->ContinueStack.Top;
	idx_before = parser->Output.Pos;
	NANOC_NEXT();
	NANOC_PROPAGATE(loop_nest(parser));
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JMP, idx_before);
	break_continue_addr(parser, prev_break, prev_continue, idx_before);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_for(NanoC_Parser *parser)
{
	NanoC_Address idx_before, idx_branch, idx_continue, idx_skip_inc;
	size_t var_cnt, prev_break, prev_continue;
	NanoC_TokenType tt;

	var_cnt = nanoc_map_count(&parser->Variables);
	prev_break = parser->BreakStack.Top;
	prev_continue = parser->ContinueStack.Top;
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NANOC_NEXT();
	tt = NANOC_TT(0);
	if(tt != NANOC_TT_SEMICOLON)
	{
		if(tt == NANOC_TT_INT)
		{
			NANOC_PROPAGATE(nanoc_int(parser));
		}
		else
		{
			NANOC_PROPAGATE(nanoc_substmt(parser, NANOC_TT_SEMICOLON));
		}
	}

	NANOC_NEXT();
	idx_before = parser->Output.Pos;
	idx_continue = idx_before;
	idx_branch = 0;
	if(NANOC_TT(0) != NANOC_TT_SEMICOLON)
	{
		NANOC_PROPAGATE(nanoc_expression(parser));
		idx_branch = nanoc_output_unknown_jump(
			&parser->Output, NANOC_INSTR_JZ);
	}

	idx_skip_inc = nanoc_output_unknown_jump(
		&parser->Output, NANOC_INSTR_JMP);

	NANOC_EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	NANOC_NEXT();
	if(NANOC_TT(0) != NANOC_TT_R_PAREN)
	{
		idx_continue = parser->Output.Pos;
		NANOC_PROPAGATE(nanoc_substmt(parser, NANOC_TT_R_PAREN));
		nanoc_output_jump(&parser->Output, NANOC_INSTR_JMP, idx_before);
	}

	nanoc_output_jump_here(&parser->Output, idx_skip_inc);
	NANOC_NEXT();
	NANOC_PROPAGATE(loop_nest(parser));
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JMP, idx_continue);
	if(idx_branch)
	{
		nanoc_output_jump_here(&parser->Output, idx_branch);
	}

	break_continue_addr(parser, prev_break, prev_continue, idx_continue);
	nanoc_map_reset(&parser->Variables, var_cnt);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_while(NanoC_Parser *parser)
{
	NanoC_Address idx_branch, idx_before;
	size_t prev_break, prev_continue;

	idx_before = parser->Output.Pos;
	prev_break = parser->BreakStack.Top;
	prev_continue = parser->ContinueStack.Top;
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NANOC_NEXT();
	NANOC_PROPAGATE(nanoc_expression(parser));
	NANOC_EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	idx_branch = nanoc_output_unknown_jump(&parser->Output, NANOC_INSTR_JZ);
	NANOC_NEXT();
	NANOC_PROPAGATE(loop_nest(parser));
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JMP, idx_before);
	nanoc_output_emit16_at(&parser->Output, idx_branch, parser->Output.Pos);
	break_continue_addr(parser, prev_break, prev_continue, idx_before);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_do_while(NanoC_Parser *parser)
{
	NanoC_Address idx_branch, idx_begin;
	size_t prev_break, prev_continue;

	idx_begin = parser->Output.Pos;
	prev_break = parser->BreakStack.Top;
	prev_continue = parser->ContinueStack.Top;
	NANOC_NEXT();
	NANOC_PROPAGATE(loop_nest(parser));
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_WHILE, NANOC_ERROR_EXPECTED_WHILE);
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_L_PAREN, NANOC_ERROR_EXPECTED_L_PAREN);
	NANOC_NEXT();
	NANOC_PROPAGATE(nanoc_expression(parser));
	NANOC_EXPECT(NANOC_TT_R_PAREN, NANOC_ERROR_EXPECTED_R_PAREN);
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	idx_branch = parser->Output.Pos;
	nanoc_output_jump(&parser->Output, NANOC_INSTR_JNZ, idx_begin);
	break_continue_addr(parser, prev_break, prev_continue, idx_branch);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_break(NanoC_Parser *parser)
{
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	if(!parser->BreakNesting)
	{
		NANOC_THROW(NANOC_ERROR_BREAK_NOT_WITHIN_LOOP_OR_SWITCH);
	}

	break_continue_jump(&parser->Output, &parser->BreakStack);
	return NANOC_STATUS_SUCCESS;
}

NanoC_Status nanoc_continue(NanoC_Parser *parser)
{
	NANOC_NEXT();
	NANOC_EXPECT(NANOC_TT_SEMICOLON, NANOC_ERROR_EXPECTED_SEMICOLON);
	if(!parser->ContinueNesting)
	{
		NANOC_THROW(NANOC_ERROR_CONTINUE_NOT_WITHIN_LOOP);
	}

	break_continue_jump(&parser->Output, &parser->ContinueStack);
	return NANOC_STATUS_SUCCESS;
}
