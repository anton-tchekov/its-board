/**
 * @file    nanoc_disassembler.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC disassembler
 */

#ifndef __NANOC_DISASM_H__
#define __NANOC_DISASM_H__

#ifdef NANOC_DESKTOP
#ifdef NANOC_DEBUG

#include "types.h"

u8r nanoc_instr_print(const u8 *program, size_t offset);
void nanoc_disasm(const u8 *program, size_t len);

#endif /* NANOC_DEBUG */
#endif /* NANOC_DESKTOP */

#endif /* __NANOC_DISASM_H__ */
