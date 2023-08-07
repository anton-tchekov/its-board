/**
 * @file    keyboard-shared.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 * @brief   Keyboard types and utilities
 */

#ifndef __KEYBOARD_SHARED_H__
#define __KEYBOARD_SHARED_H__

#include "keys.h"

/** Key State: Released (Key Up), Pressed (Key Down), Repeat (Held Down) */
typedef enum
{
	KEYSTATE_RELEASED,
	KEYSTATE_PRESSED,
	KEYSTATE_REPEAT
} KeyState;

#endif /* __KEYBOARD_SHARED_H__ */
