/**
 * @file    init.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-07-20
 * @brief   Board initialization functions
 */

#ifndef __INIT_H__
#define	__INIT_H__

#include <stdint.h>

extern uint32_t __bss_start, __bss_end,
	__data_start, __data_end, __data_si;

/**
 * @brief Initialize ITS-Board
 *        This function must be called at the beginning of the main function
 */
void its_board_init(void);

#endif /* __INIT_H__ */
