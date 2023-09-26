/**
 * @file    ps2_ll.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-24
 * @brief   PS/2 keyboard low level interface
 */

#ifndef __PS2_LL_H__
#define __PS2_LL_H__

/**
 * @brief Initialize PS/2 low level interface
 */
void ps2_ll_init(void);

/**
 * @brief Get the PS/2 data line state
 *
 * @return 0 if LOW, non-zero if HIGH
 */
int ps2_data_read(void);

#endif /* __PS2_LL_H__ */
