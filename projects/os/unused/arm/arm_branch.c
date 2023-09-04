#include "arm_branch.h"

/**
 * @brief Generate the ARM Opcode for a branch instruction
 *
 * @param cond Condition code
 * @param link If set, branch and link
 * @param cur Current address
 * @param target Target address
 */
static uint32_t arm_branch(
	uint32_t link, uint32_t cur, uint32_t target)
{
	uint32_t offset = (target - cur - 8) & ((1 << 24) - 1);

	return ((uint32_t)0x05 << 25) |
		(link << 24) |
		offset;
}

/* Branch */
static uint32_t arm_b(uint32_t cur, uint32_t target)
{
	return arm_branch(0, cur, target);
}

/* Branch and Link */
static uint32_t arm_bl(uint32_t cur, uint32_t target)
{
	return arm_branch(1, cur, target);
}

/* Branch and Exchange */
static uint32_t arm_bx(uint32_t rn)
{
	return ((uint32_t)0x12FFF1 << 4) | rn;
}
