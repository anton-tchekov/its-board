/**
 * @file    ps2.c
 * @author  Haron Nazari, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   PS/2 Keyboard Driver with Interrupts
 */

#include <stdint.h>
#include "ps2.h"
#include "ps2_ll.h"
#include "keys.h"
#include "keyboard-shared.h"
#include "layout.h"

/* --- States --- */
#define PS2_STATE_EXTRA       (1 << 0)
#define PS2_STATE_RELEASE     (1 << 1)

/* --- Control bytes --- */
#define PS2_BREAK              0xF0
#define PS2_EXTRA              0xE0

/* --- Keycodes with E0 prefix --- */
#define PS2_R_ALT              0x11
#define PS2_R_CTRL             0x14
#define PS2_L_GUI              0x1F
#define PS2_R_GUI              0x27

#define PS2_END                0x69
#define PS2_HOME               0x6C
#define PS2_INSERT             0x70
#define PS2_DELETE             0x71
#define PS2_UP                 0x75
#define PS2_DOWN               0x72
#define PS2_RIGHT              0x74
#define PS2_LEFT               0x6B
#define PS2_PAGE_UP            0x7D
#define PS2_PAGE_DOWN          0x7A

#define PS2_KP_DIVIDE          0x4A
#define PS2_KP_ENTER           0x5A

/* -- 0 to 9 --- */
#define PS2_0                  0x45
#define PS2_1                  0x16
#define PS2_2                  0x1E
#define PS2_3                  0x26
#define PS2_4                  0x25
#define PS2_5                  0x2E
#define PS2_6                  0x36
#define PS2_7                  0x3D
#define PS2_8                  0x3E
#define PS2_9                  0x46

/* --- A to Z --- */
#define PS2_A                  0x1C
#define PS2_B                  0x32
#define PS2_C                  0x21
#define PS2_D                  0x23
#define PS2_E                  0x24
#define PS2_F                  0x2B
#define PS2_G                  0x34
#define PS2_H                  0x33
#define PS2_I                  0x43
#define PS2_J                  0x3B
#define PS2_K                  0x42
#define PS2_L                  0x4B
#define PS2_M                  0x3A
#define PS2_N                  0x31
#define PS2_O                  0x44
#define PS2_P                  0x4D
#define PS2_Q                  0x15
#define PS2_R                  0x2D
#define PS2_S                  0x1B
#define PS2_T                  0x2C
#define PS2_U                  0x3C
#define PS2_V                  0x2A
#define PS2_W                  0x1D
#define PS2_X                  0x22
#define PS2_Y                  0x35
#define PS2_Z                  0x1A

/* --- Other --- */
#define PS2_ESCAPE             0x76
#define PS2_TAB                0x0D
#define PS2_SPACE              0x29
#define PS2_BACKSPACE          0x66
#define PS2_CAPSLOCK           0x58
#define PS2_RETURN             0x5A
#define PS2_R_SHIFT            0x59

#define PS2_NON_US_BACKSLASH   0x61
#define PS2_SLASH              0x4A
#define PS2_PERIOD             0x49
#define PS2_COMMA              0x41
#define PS2_GRAVE              0x0E
#define PS2_BACKSLASH          0x5D
#define PS2_R_BRACKET          0x5B
#define PS2_L_BRACKET          0x54
#define PS2_MINUS              0x4E
#define PS2_EQUALS             0x55
#define PS2_APOSTROPHE         0x52
#define PS2_SEMICOLON          0x4C

/* --- Keypad --- */
#define PS2_KP_0               0x70
#define PS2_KP_1               0x69
#define PS2_KP_2               0x72
#define PS2_KP_3               0x7A
#define PS2_KP_4               0x6B
#define PS2_KP_5               0x73
#define PS2_KP_6               0x74
#define PS2_KP_7               0x6C
#define PS2_KP_8               0x75
#define PS2_KP_9               0x7D
#define PS2_KP_PERIOD          0x71
#define PS2_KP_PLUS            0x79
#define PS2_KP_MINUS           0x7B
#define PS2_KP_MULTIPLY        0x7C

/* --- Modifiers --- */
#define PS2_L_ALT              0x11
#define PS2_L_SHIFT            0x12
#define PS2_L_CTRL             0x14

/* --- Function Keys --- */
#define PS2_F1                 0x05
#define PS2_F2                 0x06
#define PS2_F3                 0x04
#define PS2_F4                 0x0C
#define PS2_F5                 0x03
#define PS2_F6                 0x0B
#define PS2_F7                 0x83
#define PS2_F8                 0x0A
#define PS2_F9                 0x01
#define PS2_F10                0x09
#define PS2_F11                0x78
#define PS2_F12                0x07

/** Capacity of key event buffer */
#define KBD_BUF_SIZE         16

/** Write index */
static int _buf_head;

/** Read index */
static int _buf_tail;

/** Stores the key that was pressed */
static uint16_t _key_buf[KBD_BUF_SIZE];

/** Stores the state of the key at the same index */
static uint8_t _state_buf[KBD_BUF_SIZE];

void ps2_init(void)
{
	ps2_ll_init();
}

int ps2_read(KeyEvent *e)
{
	int next;
	KeyState state;
	int key;
	int chr;

	if(_buf_head == _buf_tail)
	{
		/* Buffer empty */
		return 0;
	}

	next = _buf_tail + 1;
	if(next >= KBD_BUF_SIZE)
	{
		next = 0;
	}

	key = _key_buf[_buf_tail];
	state = _state_buf[_buf_tail];
	_buf_tail = next;

	chr = key_to_codepoint(key);
	e->Key = key;
	e->Codepoint = chr;
	e->State = state;
	return 1;
}

/**
 * @brief Add a key event to the buffer
 *
 * @param key Scancode
 * @param state State of the key
 */
static void _kbd_buffer_push(int key, int state)
{
	int next = _buf_head + 1;
	if(next >= KBD_BUF_SIZE)
	{
		next = 0;
	}

	if(next == _buf_tail)
	{
		/* Buffer full, too bad */
		return;
	}

	_key_buf[_buf_head] = key;
	_state_buf[_buf_head] = state;
	_buf_head = next;
}

/**
 * @brief Event function that is called when a byte is
 *        received from the keyboard
 *
 * @param byte The byte
 */
static void ps2_event(int byte)
{
	static int state;
	static int mods, prev;
	int key;

	/*
	 * The following code translates PS/2 keyboard scancodes to USB HID
	 * scancodes, which are then converted to characters by another
	 * equally bad piece of code. This is horrible, but at least somewhat
	 * maintainable.
	 */

	key = 0;
	if(byte == PS2_BREAK)
	{
		state |= PS2_STATE_RELEASE;
		return;
	}
	else if(byte == PS2_EXTRA)
	{
		state |= PS2_STATE_EXTRA;
		return;
	}

	if(state & PS2_STATE_EXTRA)
	{
		switch(byte)
		{
			/* --- Keycodes with E0 prefix --- */
			case PS2_R_ALT:             key = KEY_R_ALT;             break;
			case PS2_R_CTRL:            key = KEY_R_CTRL;            break;
			case PS2_L_GUI:             key = KEY_L_GUI;             break;
			case PS2_R_GUI:             key = KEY_R_GUI;             break;
			case PS2_END:               key = KEY_END;               break;
			case PS2_HOME:              key = KEY_HOME;              break;
			case PS2_INSERT:            key = KEY_INSERT;            break;
			case PS2_DELETE:            key = KEY_DELETE;            break;
			case PS2_UP:                key = KEY_UP;                break;
			case PS2_DOWN:              key = KEY_DOWN;              break;
			case PS2_RIGHT:             key = KEY_RIGHT;             break;
			case PS2_LEFT:              key = KEY_LEFT;              break;
			case PS2_PAGE_UP:           key = KEY_PAGE_UP;           break;
			case PS2_PAGE_DOWN:         key = KEY_PAGE_DOWN;         break;
			case PS2_KP_DIVIDE:         key = KEY_KP_DIVIDE;         break;
			case PS2_KP_ENTER:          key = KEY_KP_ENTER;          break;
		}

		state &= ~PS2_STATE_EXTRA;
	}
	else
	{
		switch(byte)
		{
			/* --- 0 to 9 --- */
			case PS2_0:                 key = KEY_0;                 break;
			case PS2_1:                 key = KEY_1;                 break;
			case PS2_2:                 key = KEY_2;                 break;
			case PS2_3:                 key = KEY_3;                 break;
			case PS2_4:                 key = KEY_4;                 break;
			case PS2_5:                 key = KEY_5;                 break;
			case PS2_6:                 key = KEY_6;                 break;
			case PS2_7:                 key = KEY_7;                 break;
			case PS2_8:                 key = KEY_8;                 break;
			case PS2_9:                 key = KEY_9;                 break;

			/* --- A to Z --- */
			case PS2_A:                 key = KEY_A;                 break;
			case PS2_B:                 key = KEY_B;                 break;
			case PS2_C:                 key = KEY_C;                 break;
			case PS2_D:                 key = KEY_D;                 break;
			case PS2_E:                 key = KEY_E;                 break;
			case PS2_F:                 key = KEY_F;                 break;
			case PS2_G:                 key = KEY_G;                 break;
			case PS2_H:                 key = KEY_H;                 break;
			case PS2_I:                 key = KEY_I;                 break;
			case PS2_J:                 key = KEY_J;                 break;
			case PS2_K:                 key = KEY_K;                 break;
			case PS2_L:                 key = KEY_L;                 break;
			case PS2_M:                 key = KEY_M;                 break;
			case PS2_N:                 key = KEY_N;                 break;
			case PS2_O:                 key = KEY_O;                 break;
			case PS2_P:                 key = KEY_P;                 break;
			case PS2_Q:                 key = KEY_Q;                 break;
			case PS2_R:                 key = KEY_R;                 break;
			case PS2_S:                 key = KEY_S;                 break;
			case PS2_T:                 key = KEY_T;                 break;
			case PS2_U:                 key = KEY_U;                 break;
			case PS2_V:                 key = KEY_V;                 break;
			case PS2_W:                 key = KEY_W;                 break;
			case PS2_X:                 key = KEY_X;                 break;
			case PS2_Y:                 key = KEY_Y;                 break;
			case PS2_Z:                 key = KEY_Z;                 break;

			/* --- Function Keys --- */
			case PS2_F1:                key = KEY_F1;                break;
			case PS2_F2:                key = KEY_F2;                break;
			case PS2_F3:                key = KEY_F3;                break;
			case PS2_F4:                key = KEY_F4;                break;
			case PS2_F5:                key = KEY_F5;                break;
			case PS2_F6:                key = KEY_F6;                break;
			case PS2_F7:                key = KEY_F7;                break;
			case PS2_F8:                key = KEY_F8;                break;
			case PS2_F9:                key = KEY_F9;                break;
			case PS2_F10:               key = KEY_F10;               break;
			case PS2_F11:               key = KEY_F11;               break;
			case PS2_F12:               key = KEY_F12;               break;

			/* --- Keypad --- */
			case PS2_KP_0:              key = KEY_KP_0;              break;
			case PS2_KP_1:              key = KEY_KP_1;              break;
			case PS2_KP_2:              key = KEY_KP_2;              break;
			case PS2_KP_3:              key = KEY_KP_3;              break;
			case PS2_KP_4:              key = KEY_KP_4;              break;
			case PS2_KP_5:              key = KEY_KP_5;              break;
			case PS2_KP_6:              key = KEY_KP_6;              break;
			case PS2_KP_7:              key = KEY_KP_7;              break;
			case PS2_KP_8:              key = KEY_KP_8;              break;
			case PS2_KP_9:              key = KEY_KP_9;              break;
			case PS2_KP_PERIOD:         key = KEY_KP_PERIOD;         break;
			case PS2_KP_PLUS:           key = KEY_KP_PLUS;           break;
			case PS2_KP_MINUS:          key = KEY_KP_MINUS;          break;
			case PS2_KP_MULTIPLY:       key = KEY_KP_MULTIPLY;       break;

			/* --- Modifiers --- */
			case PS2_L_ALT:             key = KEY_L_ALT;             break;
			case PS2_L_SHIFT:           key = KEY_L_SHIFT;           break;
			case PS2_L_CTRL:            key = KEY_L_CTRL;            break;

			/* --- Other --- */
			case PS2_ESCAPE:            key = KEY_ESCAPE;            break;
			case PS2_TAB:               key = KEY_TAB;               break;
			case PS2_SPACE:             key = KEY_SPACE;             break;
			case PS2_BACKSPACE:         key = KEY_BACKSPACE;         break;
			case PS2_CAPSLOCK:          key = KEY_CAPSLOCK;          break;
			case PS2_RETURN:            key = KEY_RETURN;            break;
			case PS2_R_SHIFT:           key = KEY_R_SHIFT;           break;

			case PS2_NON_US_BACKSLASH:  key = KEY_NON_US_BACKSLASH;  break;

			case PS2_SLASH:             key = KEY_SLASH;             break;
			case PS2_PERIOD:            key = KEY_PERIOD;            break;
			case PS2_COMMA:             key = KEY_COMMA;             break;
			case PS2_GRAVE:             key = KEY_GRAVE;             break;
			case PS2_BACKSLASH:         key = KEY_BACKSLASH;         break;
			case PS2_R_BRACKET:         key = KEY_R_BRACKET;         break;
			case PS2_L_BRACKET:         key = KEY_L_BRACKET;         break;
			case PS2_MINUS:             key = KEY_MINUS;             break;
			case PS2_EQUALS:            key = KEY_EQUALS;            break;
			case PS2_APOSTROPHE:        key = KEY_APOSTROPHE;        break;
			case PS2_SEMICOLON:         key = KEY_SEMICOLON;         break;
		}
	}

	if(key)
	{
		int mod;
		KeyState ks;

		/* handle modifiers */
		mod = 0;
		if(key == KEY_L_ALT)
		{
			mod = MOD_ALT;
		}
		else if(key == KEY_R_ALT)
		{
			mod = MOD_ALT_GR;
		}
		else if(key == KEY_L_SHIFT || key == KEY_R_SHIFT)
		{
			mod = MOD_SHIFT;
		}
		else if(key == KEY_L_CTRL || key == KEY_R_CTRL)
		{
			mod = MOD_CTRL;
		}
		else if(key == KEY_L_GUI || key == KEY_R_GUI)
		{
			mod = MOD_OS;
		}

		if(mod)
		{
			if(state & PS2_STATE_RELEASE)
			{
				mods &= ~mod;
			}
			else
			{
				mods |= mod;
			}
		}

		/* trigger key event */
		key |= mods;

		if(state & PS2_STATE_RELEASE)
		{
			ks = KEYSTATE_RELEASED;
			prev = 0;
		}
		else
		{
			if(key == prev)
			{
				ks = KEYSTATE_REPEAT;
			}
			else
			{
				prev = key;
				ks = KEYSTATE_PRESSED;
			}
		}

		_kbd_buffer_push(key, ks);
	}

	state &= ~PS2_STATE_RELEASE;
}

void ps2_clock_falling_edge(void)
{
	/** The byte that is currently being received */
	static int byte;

	/**
	 * The number of the current bit:
	 *
	 *   0: Start bit
	 * 1-8: Data bit
	 *   9: Parity bit
	 *  10: Stop bit
	 */
	static int bit;

	if(bit == 0)
	{
		/* Start bit, reset byte */
		byte = 0;
	}
	else if(bit >= 1 && bit <= 8)
	{
		/* Data bit */
		if(ps2_data_read())
		{
			/* Set bit */
			byte |= (1 << (bit - 1));
		}
	}
	else if(bit == 9)
	{
		/* Ignore parity bit */
		ps2_event(byte);
	}
	else if(bit == 10)
	{
		/* Stop bit, reset bit counter */
		bit = 0;

		/* Return immediately so it stays 0 */
		return;
	}

	++bit;
}
