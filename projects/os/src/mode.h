/**
 * @file    mode.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Switching between apps
 */

#ifndef __MODE_H__
#define __MODE_H__

/** Enum of all modes/apps */
enum
{
	MODE_SHELL,
	MODE_EDITOR,
};

/**
 * @brief Switch mode
 *
 * @param mode One of the enum values above
 */
void mode_set(int mode);

/**
 * @brief Forward a key press to an app
 *
 * @param key Key code
 * @param c Char code (ASCII/Unicode)
 */
void mode_key(int key, int c);

#endif /* __MODE_H__ */
