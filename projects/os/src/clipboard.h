/**
 * @file    clipboard.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 * @brief   Global clipboard storage
 */

#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

/** Maximum number of bytes in clipboard */
#define CLIPBOARD_MAX_LENGTH 512

/**
 * @brief Get clipboard contents
 *
 * @param len Output pointer for the length of the clipboard data
 * @return Read-only pointer to clipboard data
 */
const char *clipboard_get(int *len);

/**
 * @brief Save text to clipboard
 *
 * @param text Text to save (is copied)
 * @param len Length of the text to save
 * @return 0 on success, 1 if text is too long
 */
int clipboard_save(const char *text, int len);

#endif /* __CLIPBOARD_H__ */
