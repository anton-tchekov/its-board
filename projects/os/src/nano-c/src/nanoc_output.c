#include "nanoc_output.h"
#include "nanoc_util.h"
#include "nanoc_instruction.h"

void nanoc_output_init(NanoC_Output *output, u8 *buffer, size_t size)
{
	output->Buffer = buffer;
	output->Pos = 0;
	output->Size = size;
}

void nanoc_output_emit(NanoC_Output *output, u8r val)
{
	output->Buffer[output->Pos] = val;
	output->Pos += 1;
}

void nanoc_output_emit2(NanoC_Output *output, u8r instr, u8r val)
{
	nanoc_output_emit(output, instr);
	nanoc_output_emit(output, val);
}

void nanoc_output_emit16(NanoC_Output *output, u16r val)
{
	write_16(output->Buffer + output->Pos, val);
	output->Pos += 2;
}

void nanoc_output_emit32(NanoC_Output *output, u32r val)
{
	write_32(output->Buffer + output->Pos, val);
	output->Pos += 4;
}

void nanoc_output_emit16_at(NanoC_Output *output, u32r loc, u16r val)
{
	write_16(output->Buffer + loc, val);
}

void nanoc_output_pushi(NanoC_Output *output, u32 value)
{
	if(value <= 0xFF)
	{
		nanoc_output_emit(output, NANOC_INSTR_PUSHI8);
		nanoc_output_emit(output, value);
	}
	else if(value <= 0xFFFF)
	{
		nanoc_output_emit(output, NANOC_INSTR_PUSHI16);
		nanoc_output_emit16(output, value);
	}
	else
	{
		nanoc_output_emit(output, NANOC_INSTR_PUSHI32);
		nanoc_output_emit32(output, value);
	}
}

NanoC_Address nanoc_output_unknown_jump(NanoC_Output *output, u8r instr)
{
	NanoC_Address idx;
	nanoc_output_emit(output, instr);
	idx = output->Pos;
	output->Pos += 2;
	return idx;
}

void nanoc_output_jump(NanoC_Output *output, u8r instr, NanoC_Address addr)
{
	nanoc_output_emit(output, instr);
	nanoc_output_emit16(output, addr);
}

void nanoc_output_jump_here(NanoC_Output *output, NanoC_Address addr)
{
	nanoc_output_emit16_at(output, addr, output->Pos);
}
