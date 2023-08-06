#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>

void platform_init(void);
void platform_blit(uint8_t *screen);
void platform_palette(uint8_t *chg);
void platform_destroy(void);

#endif /* __PLATFORM_H__ */
