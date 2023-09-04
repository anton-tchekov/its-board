#ifndef __CTYPE_H__
#define __CTYPE_H__

static inline int isprint(int c)
{
	return c >= 32 && c <= 126;
}

#endif /* __CTYPE_H__ */
