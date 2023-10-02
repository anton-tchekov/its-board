/**
 * @file    manager.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   File manager similar to midnight commander
 */

#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "types.h"

void manager_init(void);
void manager_open(void);
void manager_key(int key, int c);

#endif /* __MANAGER_H__ */
