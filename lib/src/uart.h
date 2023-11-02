/**
 * @file    uart.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-11-01
 * @brief   UART functions
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

/**
 * @brief Initialize the UART at a certain baud rate
 *
 * @param baud Baud rate
 */
void uart_init(uint32_t baud);

/**
 * @brief Transmit a byte over the UART
 *
 * @param val The byte to transmit
 */
void uart_tx(int val);

/**
 * @brief Transmit a string over the UART
 *
 * @param s The null-terminated string to send
 */
void uart_tx_str(const char *s);

#endif /* __UART_H__ */
