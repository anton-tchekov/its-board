/**
 * @file    overlay.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "overlay.h"

void alert(const char *msg, void (*callback)(void))
{

}

void confirm(const char *msg, void (*callback)(int))
{

}

void prompt(const char *msg, void (*callback)(int, char *))
{

}

void overlay_key(int key, int c)
{
	(void)key, (void)c;
}
