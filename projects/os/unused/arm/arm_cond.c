#include "arm_cond.h"

ARM_Instruction arm_set_cond(ARM_Instruction instr, ARM_Condition cond)
{
	return instr | (cond << 28);
}
