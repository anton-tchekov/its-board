#ifndef __PS2_H__
#define __PS2_H__

#include <stdint.h>

typedef struct
{
	uint32_t Codepoint;
	uint16_t Key;
	uint8_t State;
} KeyEvent;

/**
 * @brief Initialize PS/2 keyboard driver and interrupt, Interrupts must be
 *        enabled afterwards
 */
void ps2_init(void);

/**
 * @brief Get buffered key events
 *
 * @param e Key event data
 * @return true if there was a key event in the buffer
 */
int ps2_read(KeyEvent *e);

/**
 * @brief Called when a falling edge is detected on the PS/2 clock pin
 */
void ps2_clock_falling_edge(void);

#endif /* __PS2_H__ */
