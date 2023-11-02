/**
 * @file    GP2Y0A21.c
 * @author  Haron Nazari, Anton Tchekov
 * @version 0.1
 * @date    2023-10-26
 */

#include "GP2Y0A21.h"
#include <stddef.h>

#define XXXX 0

int32_t gp2_get_distance(uint32_t value)
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

		{ XXXX, 100 },
		{ XXXX, 110 },
		{ XXXX, 120 },
		{ XXXX, 130 },
		{ XXXX, 140 },
		{ XXXX, 150 },
		{ XXXX, 160 },
		{ XXXX, 170 },
		{ XXXX, 180 },
		{ XXXX, 190 },
		{ XXXX, 200 },
		{ XXXX, 210 },
		{ XXXX, 220 },
		{ XXXX, 230 },
		{ XXXX, 240 },
		{ XXXX, 250 },
		{ XXXX, 260 },
		{ XXXX, 270 },
		{ XXXX, 280 },
		{ XXXX, 290 },
		{ XXXX, 300 },
		{ XXXX, 310 },
		{ XXXX, 320 },
		{ XXXX, 330 },
		{ XXXX, 340 },
		{ XXXX, 350 },
		{ XXXX, 360 },
		{ XXXX, 370 },
		{ XXXX, 380 },
		{ XXXX, 390 },
		{ XXXX, 400 },
		{ XXXX, 410 },
		{ XXXX, 420 },
		{ XXXX, 430 },
		{ XXXX, 440 },
		{ XXXX, 450 },
		{ XXXX, 460 },
		{ XXXX, 470 },
		{ XXXX, 480 },
		{ XXXX, 490 },
		{ XXXX, 500 },
		{ XXXX, 510 },
		{ XXXX, 520 },
		{ XXXX, 530 },
		{ XXXX, 540 },
		{ XXXX, 550 },
		{ XXXX, 560 },
		{ XXXX, 570 },
		{ XXXX, 580 },
		{ XXXX, 590 },
		{ XXXX, 600 },
		{ XXXX, 610 },
		{ XXXX, 620 },
		{ XXXX, 630 },
		{ XXXX, 640 },
		{ XXXX, 650 },
		{ XXXX, 660 },
		{ XXXX, 670 },
		{ XXXX, 680 },
		{ XXXX, 690 },
		{ XXXX, 700 },
		{ XXXX, 710 },
		{ XXXX, 720 },
		{ XXXX, 730 },
		{ XXXX, 740 },
		{ XXXX, 750 },
		{ XXXX, 760 },
		{ XXXX, 770 },
		{ XXXX, 780 },
		{ XXXX, 790 },
		{ XXXX, 800 },
	};

	size_t i;
	uint32_t v1, v2, m1, m2;

	/* Loop over all pairs of adjacent points */
	for(i = 0; i < sizeof(gp) / sizeof(*gp) - 1; ++i)
	{
		v1 = gp[i].Value;
		v2 = gp[i + 1].Value;

		/* Is value between two known points */
		if(v1 >= value && v2 <= value)
		{
			m1 = gp[i].MM;
			m2 = gp[i + 1].MM;

			/* Linear interpolation */
			return m1 + (value - v1) * (m2 - m1) / (v2 - v1);
		}
	}

	return -1;
}
