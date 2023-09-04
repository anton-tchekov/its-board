#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#define CLIPBOARD_MAX_LENGTH 512

const char *clipboard_get(int *len);
int clipboard_save(const char *text, int len);

#endif /* __CLIPBOARD_H__ */
