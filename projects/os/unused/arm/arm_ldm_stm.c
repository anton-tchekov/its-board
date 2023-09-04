#include "arm_ldm_stm.h"

/**
 * @param rn Base register
 * @param register_list List of Registers to Load/Store
 * @return ARM Instruction
 */
ARM_Instruction arm_ldm_stm(ARM_Register rn, ARM_RegisterList register_list)
{
	return (0x4 << 25) |
		(rn << 16) |
		register_list;
}
