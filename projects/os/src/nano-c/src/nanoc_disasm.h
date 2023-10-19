/**
 * @file    nanoc_disasm.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-09
 * @brief   NanoC disassembler
 */

#ifndef __NANOC_DISASM_H__
#define __NANOC_DISASM_H__

#ifdef NANOC_DESKTOP
#ifdef NANOC_DEBUG

#include "nanoc_types.h"

/**
 * @brief Disassemble and print the instruction at the offset
 *        in the bytecode
 *
 * @param program Bytecode
 * @param offset Byte offset
 * @return Size of the disassembled instruction in bytes
 */
size_t nanoc_instr_print(const u8 *program, size_t offset);

/**
 * @brief Disassemble and print all instructions
 *
 * @param program Bytecode
 * @param len Length of program in bytes
 */
void nanoc_disasm(const u8 *program, size_t len);

#endif /* NANOC_DEBUG */
#endif /* NANOC_DESKTOP */

#endif /* __NANOC_DISASM_H__ */
