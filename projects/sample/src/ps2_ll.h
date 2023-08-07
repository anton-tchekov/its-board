#ifndef __PS2_LL_H__
#define __PS2_LL_H__

void ps2_ll_init(void);

/**
 * @brief Get the PS/2 data line state
 *
 * @return 0 if LOW, non-zero if HIGH
 */
int ps2_data_read(void);

#endif /* __PS2_LL_H__ */
