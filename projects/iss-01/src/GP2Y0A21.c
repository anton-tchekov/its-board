/**
 * @file    GP2Y0A21.c
 * @author  Haron Nazari, Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 */

#include "GP2Y0A21.h"
#include <stddef.h>

#define XXXX 0

int32_t gp2_get_distance(int32_t value)
{
	/* Mapping from a ADC value to millimeters */
	typedef struct
	{
		uint16_t Value;
		uint16_t MM;
	} GraphPoint;

	static const GraphPoint gp[] =
	{
		/* Experimental values from Sensor 03 */
		/*{ 3150,  60 },
		{ 2950,  70 },
		{ 2730,  80 },
		{ 2310, 100 },
		{ 1640, 150 },
		{ 1310, 200 },
		{ 1080, 250 },
		{  920, 300 },
		{  740, 400 },
		{  610, 500 },
		{  520, 600 },
		{  450, 700 },
		{  400, 800 },*/

		{ 2600, 100 },
		{ 1750, 150 },
		{ 1300, 200 },
		{ 1050, 250 },
		{  915, 300 },
		{  840, 350 },
		{  818, 400 },
		{  750, 450 },
		{  690, 500 },
		{  608, 550 },
		{  560, 600 },
	};

	size_t i;
	int32_t v1, v2, m1, m2;

	/* Loop over all pairs of adjacent points */
	for(i = 0; i < sizeof(gp) / sizeof(*gp) - 1; ++i)
	{
		v1 = gp[i].Value;
		v2 = gp[i + 1].Value;

		/* Is value between two known points */
		if(value <= v1 && value >= v2)
		{
			m1 = gp[i].MM;
			m2 = gp[i + 1].MM;

			/* Linear interpolation */
			return m1 + (value - v1) * (m2 - m1) / (v2 - v1);
		}
	}

	return -1;
}
