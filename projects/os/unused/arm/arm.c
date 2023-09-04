#include "arm.h"
#include <stdint.h>




enum ARM_ALU_OPERATION
{
	OP_AND, /* And */
	OP_EOR, /* Exclusive Or */
	OP_SUB, /* Subtract */
	OP_RSB, /* Reverse Subtract */
	OP_ADD, /* Add */
	OP_ADC, /* Add with carry */
	OP_SBC, /* Subtract with carry */
	OP_RSC, /* Reverse Subtract with carry */
	OP_TST, /* Test with And */
	OP_TEQ, /* Test with Exclusive Or */
	OP_CMP, /* Compare */
	OP_CMN, /* Compare Negated */
	OP_ORR, /* Or */
	OP_MOV, /* Move */
	OP_BIC, /* Bit clear */
	OP_MVN  /* Move Negated */
};

enum ARM_SHIFT_TYPE
{
	OP_LSL = 0x0, /* Logical Shift Left */
	OP_LSR = 0x1, /* Logical Shift Right */
	OP_ASR = 0x2, /* Arithmetic Shift Right */
	OP_ROR = 0x3  /* Rotate Right */
};


#define COND_OFFSET         28
#define ACCUMULATE_OFFSET   21
#define SET_FLAGS_OFFSET    20


ARM_Instruction arm_mul(int s,
	ARM_Register rd, ARM_Register rs, ARM_Register rm)
{
	/* TODO */
	return 0;

}

ARM_Instruction arm_mla(int s,
	ARM_Register rd, ARM_Register rn, ARM_Register rs, ARM_Register rm)
{
	/* TODO */
	return (((ARM_Instruction)1) << ACCUMULATE_OFFSET);
}

ARM_Instruction arm_ldr(ARM_Register rn, ARM_Register rd)
{
	return 0;
}

uint32_t arm_imm(
	uint32_t rd, uint32_t s, uint32_t rn, uint32_t val)
{
	uint32_t op2 = 0;

	return (s << 20) |
		(rn << 16) |
		(rd << 12) |
		op2;
}

/**
 * @brief Checks if the argument is a valid immediate shift amount
 *
 * @param v Shift amount to check
 * @return 1 if the shift amount is valid
 */
static uint32_t valid_imm_shift_amount(uint32_t v)
{
	return v < 32;
}

/**
 * @brief Checks if the argument is a valid immediate operand
 *
 * @param v Value to check
 * @return 1 if the value can be used as an immediate operand
 */
static uint32_t valid_imm_operand(uint32_t v)
{
	return 1;
}

/**
 * @brief Generate the ARM Opcode for a data processing
 *        instruction with register operands and a shift
 *        amount stored in a register
 *
 * @param cond Condition code
 * @param op Arithmetic / Logic Function
 * @param s Set condition codes
 * @param rd Destination register
 * @param rn First operand register
 * @param rm Second operand register
 * @param rs Register that contains the shift amount
 * @param shift_type Shift type
 */
static uint32_t arm_reg_shift_reg(
	uint32_t op, uint32_t s,
	uint32_t rd, uint32_t rn, uint32_t rm,
	uint32_t rs, uint32_t shift_type)
{
	return (op << 21) |
		(s << 20) |
		(rn << 16) |
		(rd << 12) |
		(rs << 8) |
		(shift_type << 5) |
		(1 << 4) |
		rm;
}

/**
 * @brief Generate the ARM Opcode for a data processing
 *        instruction with register operands and an
 *        immediate shift amount
 *
 * @param cond Condition code
 * @param op Arithmetic / Logic Function
 * @param s Set condition codes
 * @param rd Destination register
 * @param rn First operand register
 * @param rm Second operand register
 * @param shift Immediate shift amount
 * @param shift_type Shift type
 */
static uint32_t arm_reg_shift_imm(
	uint32_t op, uint32_t s,
	uint32_t rd, uint32_t rn, uint32_t rm,
	uint32_t shift, uint32_t shift_type)
{
	return (op << 21) |
		(s << 20) |
		(rn << 16) |
		(rd << 12) |
		(shift << 8) |
		(shift_type << 5) |
		rm;
}

