#include "nanoc_float.h"
#include <math.h>
#include <string.h>

/* memcpy needed because of strict aliasing rules */
static inline i32 fbti(f32 p)
{
	u32 i;
	memcpy(&i, &p, 4);
	return i;
}

static inline f32 ibtf(i32 p)
{
	f32 f;
	memcpy(&f, &p, 4);
	return f;
}

u32 nanoc_itf(u32 *p)
{
	return fbti((f32)p[0]);
}

u32 nanoc_fti(u32 *p)
{
	return (u32)ibtf(p[0]);
}

u32 nanoc_addf(u32 *p)
{
	return fbti(ibtf(p[0]) + ibtf(p[1]));
}

u32 nanoc_subf(u32 *p)
{
	return fbti(ibtf(p[0]) - ibtf(p[1]));
}

u32 nanoc_mulf(u32 *p)
{
	return fbti(ibtf(p[0]) * ibtf(p[1]));
}

u32 nanoc_divf(u32 *p)
{
	return fbti(ibtf(p[0]) / ibtf(p[1]));
}

u32 nanoc_modf(u32 *p)
{
	return fbti(fmod(ibtf(p[0]), ibtf(p[1])));
}

u32 nanoc_ltf(u32 *p)
{
	return ibtf(p[0]) < ibtf(p[1]);
}

u32 nanoc_gtf(u32 *p)
{
	return ibtf(p[0]) > ibtf(p[1]);
}

u32 nanoc_lef(u32 *p)
{
	return ibtf(p[0]) <= ibtf(p[1]);
}

u32 nanoc_gef(u32 *p)
{
	return ibtf(p[0]) >= ibtf(p[1]);
}

u32 nanoc_absf(u32 *p)
{
	return fbti(fabs(ibtf(p[0])));
}

u32 nanoc_sin(u32 *p)
{
	return fbti(sin(ibtf(p[0])));
}

u32 nanoc_cos(u32 *p)
{
	return fbti(cos(ibtf(p[0])));
}

u32 nanoc_tan(u32 *p)
{
	return fbti(tan(ibtf(p[0])));
}

u32 nanoc_asin(u32 *p)
{
	return fbti(asin(ibtf(p[0])));
}

u32 nanoc_acos(u32 *p)
{
	return fbti(acos(ibtf(p[0])));
}

u32 nanoc_atan(u32 *p)
{
	return fbti(atan(ibtf(p[0])));
}

u32 nanoc_atan2(u32 *p)
{
	return fbti(atan2(ibtf(p[0]), ibtf(p[1])));
}

u32 nanoc_sinh(u32 *p)
{
	return fbti(sinh(ibtf(p[0])));
}

u32 nanoc_cosh(u32 *p)
{
	return fbti(cosh(ibtf(p[0])));
}

u32 nanoc_tanh(u32 *p)
{
	return fbti(tanh(ibtf(p[0])));
}

u32 nanoc_exp(u32 *p)
{
	return fbti(exp(ibtf(p[0])));
}

u32 nanoc_log(u32 *p)
{
	return fbti(log(ibtf(p[0])));
}

u32 nanoc_log10(u32 *p)
{
	return fbti(log10(ibtf(p[0])));
}

u32 nanoc_pow(u32 *p)
{
	return fbti(pow(ibtf(p[0]), ibtf(p[1])));
}

u32 nanoc_sqrt(u32 *p)
{
	return fbti(sqrt(ibtf(p[0])));
}

u32 nanoc_ceil(u32 *p)
{
	return fbti(ceil(ibtf(p[0])));
}

u32 nanoc_floor(u32 *p)
{
	return fbti(floor(ibtf(p[0])));
}

u32 nanoc_round(u32 *p)
{
	return fbti(round(ibtf(p[0])));
}
