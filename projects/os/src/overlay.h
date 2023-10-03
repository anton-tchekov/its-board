/**
 * @file    overlay.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Alert, confirm, prompt functions
 */

#ifndef __OVERLAY_H__
#define __OVERLAY_H__

enum
{
	OVERLAY_NORMAL,
	OVERLAY_ERROR
};

void alert(int type, void (*callback)(void),
	const char *msg, ...);

void confirm(int type, void (*callback)(int),
	const char *msg, ...);

void prompt(int type, void (*callback)(int, char *),
	const char *msg, ...);

void overlay_key(int key, int c);

#endif /* __OVERLAY_H__ */
