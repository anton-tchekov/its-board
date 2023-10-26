#include "GP2Y0A21YK.h"

// Get the amount of elements in an array
#define elements_in_array(array) (int32_t)(sizeof(array) / sizeof(array[0]))

/*
 Voltage values for the corresponding GRAPH_POINTS_CM array
 the voltage values are written in 10mv
 For example : 315 = 3.15v
*/
static const int32_t GRAPH_POINTS_VOLTAGE[] =
{
  315, 295, 273, 231, 164, 131, 108, 92, 74, 61, 52, 45, 40
};

/*
  Distance values for the corresponding GRAPH_POINTS_VOLTAGE array
  the distance values are written in mm
  for example : 300 = 300mm
*/
static const int32_t GRAPH_POINTS_MM[] =
{
  60, 70, 80, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800
};

/**
 * @brief Acts as a Linear interpolation formula, thats supposed to help
 * approximate points that are in between 2 graph points if the function is unknown.
 * 
 * This function is to be used if you have the y value but want to find out the 
 * corresponding x value between 2 known x values and their given y values
 * 
 * @param y the point you wanna know the x value of
 * 
 * @param y1 the y value for x1
 * @param y2 the y value for x2
 * 
 * @param x1 the closest dot to the left, that you know the y value for
 * @param x2 the closest dot to the right, that you know the y value for 
 * 
 * @retval result of the linear interpolation, gives the approximate x for the given y
 *
*/
static int32_t linear_interpolation_for_y(int32_t y, int32_t y1, int32_t y2, int32_t x1, int32_t x2)
{
  return (x1 + (y - y1) * (x2 - x1) / (y2 - y1));
}

/**
 * @brief gets the closest voltage to the left of the given voltage in the GRAPH_POINTS_VOLTAGE[] array
 * @param voltage the given voltage you want to find the closest left value to
 * @retval the closest left voltage in 10mv
*/
static int32_t closest_voltage_left(int32_t voltage)
{
  // Start with the most left point
  int32_t closest_left = GRAPH_POINTS_VOLTAGE[0];

  int32_t current_v_point = 0;
  int32_t result = 0;

  // Iterate through the graph_points_array
  for(int32_t i = 0; i < elements_in_array(GRAPH_POINTS_VOLTAGE); i++)
  {
    // if theres an element smaller than the current most left voltage, but bigger than 
    // the given voltage, make it the new closet_left voltage
    current_v_point = GRAPH_POINTS_VOLTAGE[i];
    if(current_v_point < closest_left && current_v_point > voltage)
    {
      closest_left = current_v_point;
      result = current_v_point;
    }
  }

  return result;
}

/**
 * @brief gets the closest voltage to the right of the given voltage in the GRAPH_POINTS_VOLTAGE[] array
 * @param voltage the given voltage you want to find the closest right value to
 * @retval the closest right voltage in 10mv
*/
static int32_t closest_voltage_right(int32_t voltage)
{
  // Start with the most right point
  int32_t closest_right = GRAPH_POINTS_VOLTAGE[elements_in_array(GRAPH_POINTS_VOLTAGE)];

  int32_t current_v_point = 0;
  int32_t result = 0;

  // Iterate through the graph_points_array
  for(int32_t i = 0; i < elements_in_array(GRAPH_POINTS_VOLTAGE); i++)
  {
    // if theres an element bigger than the current most right voltage, but smaller than 
    // the given voltage, make it the new closest_right voltage
    current_v_point = GRAPH_POINTS_VOLTAGE[i];
    if(current_v_point > closest_right && current_v_point < voltage)
    {
      closest_right = current_v_point;
      result = current_v_point;
    }
  }

  return result;
}

/**
 * @brief gets the index of a given voltage in the GRAPH_POINTS_VOLTAGE
 * @param voltage the voltage you wanna find the index of
 * @retval the index, if it was found in the array and -1 if it wasnt found in the array
*/
static int32_t get_voltage_index(int32_t voltage)
{
  for(int32_t i = 0; i < elements_in_array(GRAPH_POINTS_VOLTAGE); i++)
  {
    if(voltage == GRAPH_POINTS_VOLTAGE[i])
    {
      return i;
    }
  }

  return -1;
}

int32_t GP2_read_voltage()
{
  // TODO : ADC Funktion

  return 0;
}

int32_t GP2_get_distance(int32_t voltage)
{
  int32_t y = voltage;
  int32_t y1 = closest_voltage_left(voltage);
  int32_t y2 = closest_voltage_right(voltage);

  int32_t x1 = GRAPH_POINTS_MM[get_voltage_index(y1)];
  int32_t x2 = GRAPH_POINTS_MM[get_voltage_index(y2)];

  int32_t distance = linear_interpolation_for_y(y, y1, y2, x1, x2);

  return distance;
}
