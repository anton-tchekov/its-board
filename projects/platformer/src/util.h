#ifndef __UTIL_H__
#define __UTIL_H__

#define SCALE_SHIFT              16
#define SCALE_FACTOR               (1 << SCALE_SHIFT)
#define FP(A,B)                    (A * SCALE_FACTOR + B * SCALE_FACTOR / 100)

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static inline int abs(int v)
{
	return v < 0 ? -v : v;
}

static inline int scale(int v)
{
	return v >> SCALE_SHIFT;
}

static inline int unscale(int v)
{
	return v << SCALE_SHIFT;
}

#endif /* __UTIL_H__ */
