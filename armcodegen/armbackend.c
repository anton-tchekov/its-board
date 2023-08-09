#include <stdint.h>

enum
{
	COND_EQ = 0x0, /* Equal (Z == 1) */
	COND_NE = 0x1, /* Not Equal (Z == 0) */
	COND_CS = 0x2, /* Carry Set = Unsigned Higher or Same (C == 1) */
	COND_HS = COND_CS,
	COND_CC = 0x3, /* Carry Clear = Unsigned Lower (C == 0) */
	COND_LO = COND_CC,
	COND_MI = 0x4, /* Negative (N == 1) */
	COND_PL = 0x5, /* Positive (N == 0) */
	COND_VS = 0x6, /* Overflow Set (V == 1) */
	COND_VC = 0x7, /* Overflow Clear (V == 0) */
	COND_HI = 0x8, /* Unsigned Higher (C == 1) && (Z == 0) */
	COND_LS = 0x9, /* Unsigned Lower or Same (C == 0) || (Z == 0) */
	COND_GE = 0xA, /* Signed Greater or Equal (N == V) */
	COND_LT = 0xB, /* Signed Less Than (N != V) */
	COND_GT = 0xC, /* Signed Greater Than (Z == 0) && (N == V) */
	COND_LE = 0xD, /* Signed Less or Equal (Z == 1) || (N != V) */
	COND_AL = 0xE, /* Always */
};

enum
{
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	FP = R11, /* Frame Pointer */
	R12,
	IP = R12, /* Interprocedural Scratch Register */
	R13,
	SP = R13, /* Stack Pointer */
	R14,
	LR = R14, /* Link Register */
	R15,
	PC = R15  /* Program Counter */
};

enum
{
	OP_AND, /* And */
	OP_EOR, /* Exclusive Or */
	OP_SUB, /* Subtract */
	OP_RSB, /* Reverse Subtract */
	OP_ADD,
	OP_ADC,
	OP_SBC,
	OP_RSC,
	OP_TST,
	OP_TEQ,
	OP_CMP,
	OP_CMN,
	OP_ORR,
	OP_MOV,
	OP_BIC,
	OP_MVN,
};

//void arm_r(int cond, );

#define COND_OFFSET 28

uint32_t arm_imm(
	uint32_t cond, uint32_t rd, uint32_t s, uint32_t rn, uint32_t val)
{
	uint32_t op2 = 0;

	return (cond << COND_OFFSET) |
		(s << 20) |
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
static uint32_t valid_imm_shift_amount(uint32_t v)
{
	return v < 32;
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
	uint32_t cond, uint32_t op, uint32_t s,
	uint32_t rd, uint32_t rn, uint32_t rm,
	uint32_t rs, uint32_t shift_type)
{
	return (cond << COND_OFFSET) |
		(op << 21) |
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
	uint32_t cond, uint32_t op, uint32_t s,
	uint32_t rd, uint32_t rn, uint32_t rm,
	uint32_t shift, uint32_t shift_type)
{
	return (cond << COND_OFFSET) |
		(op << 21) |
		(s << 20) |
		(rn << 16) |
		(rd << 12) |
		(shift << 8) |
		(shift_type << 5) |
		rm;
}

/**
 * @brief Generate the ARM Opcode for a branch instruction
 *
 * @param cond Condition code
 * @param link If set, branch and link
 * @param cur Current address
 * @param target Target address
 */
static uint32_t arm_branch(
	uint32_t cond, uint32_t link, uint32_t cur, uint32_t target)
{
	uint32_t offset = (target - cur - 8) & ((1 << 24) - 1);

	return (cond << COND_OFFSET) |
		((uint32_t)0x05 << 25) |
		(link << 24) |
		offset;
}

/* Branch */
static uint32_t arm_b(uint32_t cond, uint32_t cur, uint32_t target)
{
	return arm_branch(cond, 0, cur, target);
}

/* Branch and Link */
static uint32_t arm_bl(uint32_t cond, uint32_t cur, uint32_t target)
{
	return arm_branch(cond, 1, cur, target);
}

/* Branch and Exchange */
static uint32_t arm_bx(uint32_t cond, uint32_t rn)
{
	return (cond << COND_OFFSET) |
		((uint32_t)0x12FFF1 << 4) | rn;
}




static void parse_line(void)
{

}

