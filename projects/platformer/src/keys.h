#ifndef __KEYS_H__
#define __KEYS_H__

typedef enum KEYS
{
	KEY_MASK_UP    = 0x1,
	KEY_MASK_LEFT  = 0x2,
	KEY_MASK_RIGHT = 0x4
} Keys;

int keys_get(void);

#endif /* __KEYS_H__ */
