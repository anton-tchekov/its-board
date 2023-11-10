/**
 * @file    editor.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Editor app
 */

#ifndef __EDITOR_H__
#define __EDITOR_H__

void editor_init(void);
void editor_open(void);
void editor_load(const char *path);
void editor_key(int key, int c);
void editor_load_cmd(const char *path);
void editor_save_cmd(char *path);

/* TODO: This is fucked */
const char *editor_buf(void);

#endif /* __EDITOR_H__ */
