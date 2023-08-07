#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#define TERMINAL_GRAY    0x52AA
#define TERMINAL_RED     0xFAAA
#define TERMINAL_GREEN   0x57EA
#define TERMINAL_YELLOW  0xFFEA
#define TERMINAL_BLUE    0x52BF
#define TERMINAL_MAGENTA 0xFABF
#define TERMINAL_CYAN    0x57FF
#define TERMINAL_WHITE   0xFFFF

void terminal_init(void);
void terminal_print(const char *str);

#endif /* __TERMINAL_H__ */
