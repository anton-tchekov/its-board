#ifndef __NANOC_OUTPUT_H__
#define __NANOC_OUTPUT_H__

#include "types.h"
#include "nanoc_types.h"

typedef struct NANOC_OUTPUT
{
	u8 *Buffer;
	size_t Pos;
	size_t Size;
} NanoC_Output;

void nanoc_output_init(NanoC_Output *output, u8 *buffer, size_t size);

void nanoc_output_emit(NanoC_Output *output, u8r val);
void nanoc_output_emit2(NanoC_Output *output, u8r instr, u8r val);
void nanoc_output_emit16(NanoC_Output *output, u16r val);
void nanoc_output_emit32(NanoC_Output *output, u32r val);

void nanoc_output_emit16_at(NanoC_Output *output, u32r loc, NanoC_Address val);

void nanoc_output_pushi(NanoC_Output *output, u32 value);
NanoC_Address nanoc_output_unknown_jump(NanoC_Output *output, u8r instr);
void nanoc_output_jump(NanoC_Output *output, u8r instr, NanoC_Address addr);
void nanoc_output_jump_here(NanoC_Output *output, NanoC_Address addr);

#endif /* __NANOC_OUTPUT_H__ */
