/**
 * @file    GP2Y0A21YK.c
 * @author  Haron Nazari
 * @version 0.1
 * @date    2023-10-26
 */

#include "GP2Y0A21YK.h"
#include <stddef.h>

/** Get the number of elements in an array */
#define ARRLEN(A) (sizeof(A) / sizeof(*A))

typedef struct
{
	/**
	 * Voltage value for the corresponding distance
	 * The voltage values are written in 10 mV
	 * For example: 315 = 3.15 V
	 */
	uint16_t Voltage;

	/**
	 * Distance value for the corresponding voltage
	 * The distance values are written in mm
	 * For example: 300 = 300 mm
	 */
	uint16_t MM;
} GraphPoint;

/** Graph points array */
static const GraphPoint gp[] =
{
	{ 315,  60 },
	{ 295,  70 },
	{ 273,  80 },
	{ 231, 100 },
	{ 164, 150 },
	{ 131, 200 },
	{ 108, 250 },
	{  92, 300 },
	{  74, 400 },
	{  61, 500 },
	{  52, 600 },
	{  45, 700 },
	{  40, 800 },
};

/**
 * @brief Acts as a Linear interpolation formula, thats supposed to help
 * approximate points that are in between 2 graph points if the function
 * is unknown.
 *
 * This function is to be used if you have the y value but want to find out the
 * corresponding x value between 2 known x values and their given y values
 *
 * @param y The point you wanna know the x value of
 *
 * @param y1 The y value for x1
 * @param y2 The y value for x2
 *
 * @param x1 The closest dot to the left, that you know the y value for
 * @param x2 The closest dot to the right, that you know the y value for
 *
 * @return Result of the linear interpolation, gives the approximate x
 *         for the given y
 */
static inline int32_t lerp_y(int32_t y,
	int32_t y1, int32_t y2, int32_t x1, int32_t x2)
{
	return (x1 + (y - y1) * (x2 - x1) / (y2 - y1));
}

/**
 * @brief Gets the closest voltage to the left of the given voltage in the
 *        graph_points[] array
 *
 * @param voltage The given voltage you want to find the closest left value to
 * @return The index into the array with the closest left voltage in 10 mV
 */
static inline int32_t closest_voltage_left(uint32_t voltage)
{
	int32_t i;

	/* Iterate through the graph_points array */
	for(i = 0; i < (int32_t)ARRLEN(gp) - 1; ++i)
	{
		if(gp[i].Voltage >= voltage &&
			gp[i + 1].Voltage <= voltage)
		{
			return i;
		}
	}

	return -1;
}

int32_t gp2_get_distance(uint32_t voltage)
{
	/* Left index */
	int32_t li = closest_voltage_left(voltage);
	if(li < 0)
	{
		return -1;
	}

	return lerp_y(voltage, gp[li].Voltage, gp[li + 1].Voltage,
		gp[li].MM, gp[li + 1].MM);
}
