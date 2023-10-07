/**
 * @file    overlay.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Alert, confirm, prompt functions
 */

#ifndef __OVERLAY_H__
#define __OVERLAY_H__

/** Overlay types enum */
enum
{
	OVERLAY_NORMAL,
	OVERLAY_ERROR
};

/**
 * @brief Display an alert with an "Ok" button
 *
 * @param type Overlay type (see enum above)
 * @param callback Callback for when the "Ok" button is pressed
 * @param msg Formatted message to display
 * @param ... Format arguments
 */
void alert(int type, void (*callback)(void),
	const char *msg, ...);

/**
 * @brief Display a confirm box with a "Yes" and a "No" button.
 *
 * @param type Overlay type (see enum above)
 * @param callback Callback for when a button is pressed
 * @param msg Formatted message to display
 * @param ... Format arguments
 */
void confirm(int type, void (*callback)(int),
	const char *msg, ...);

/**
 * @brief Display a text input prompt with a "Ok" and a "Cancel" button
 *
 * @param type Overlay type (see enum above)
 * @param callback Callback for when a button is pressed
 * @param def Default string in text field
 * @param msg Formatted message to display
 * @param ... Format arguments
 */
void prompt(int type, void (*callback)(int, char *),
	const char *def, const char *msg, ...);

void overlay_key(int key, int c);

#endif /* __OVERLAY_H__ */
