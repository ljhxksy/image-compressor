/**************************************************************
 *
 *                     rgbConversion.h
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the declaration of functions for RGB conversion to
 *    RGB Block structs and to Component Video structs.
 *
 **************************************************************/

#ifndef RGBCONVERSION_INCLUDED
#define RGBCONVERSION_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mem.h"
#include "assert.h"
#include "except.h"
#include "pnm.h"
#include "helpers.h"

RGB imageToRGB(Pnm_ppm image, int col, int row);
rgbBlock imageToRgbBlock(Pnm_ppm image, int col, int row);
void RGBtoImage(Pnm_ppm image, RGB rgb, int col, int row);
RGB compVidtoRGB(ComponentVideo cv);
rgbBlock RGBtoRGBblock(RGB rgb1, RGB rgb2,
                       RGB rgb3, RGB rgb4);
rgbBlock CompVidBlockToRgbBlock(CompVidBlock cvBlock);

#endif