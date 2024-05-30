/**************************************************************
 *
 *                     helpers.c
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the implementation of helper functions/structs
 *    that are used across the compression and decompression of ppm images
 *
 **************************************************************/
#include "helpers.h"

/********** inRange **********
 * 
 * Ensures that a given floating-point number is within a specified range.
 *
 * Parameters:
 *      float num: The input floating-point number to be checked.
 *      float min: The minimum value allowed in the range.
 *      float max: The maximum value allowed in the range.
 *
 * Returns:
 *      float: the float
 *
 * Notes: none
 * 
 ****************************/
float inRange(float num, float min, float max)
{
        if (num > max) {
                return max;
        } else if (num < min) {
                return min;
        }
        return num;
}
