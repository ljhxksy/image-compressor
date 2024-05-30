/**************************************************************
 *
 *                     compVidConversion.h
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the declaration of functions for converting 
 *    Component Videos.
 *
 **************************************************************/
#ifndef COMPVIDCONVERSION_INCLUDED
#define COMPVIDCONVERSION_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mem.h"
#include "assert.h"
#include "except.h"
#include "pnm.h"
#include "arith40.h"
#include "helpers.h"

ComponentVideo RGBtoCompVid(RGB rgb);
CompVidBlock rgbBlockToCvBlock(rgbBlock rgbBlock);
Compressed DCT(ComponentVideo cv1, ComponentVideo cv2,
               ComponentVideo cv3, ComponentVideo cv4);
CompVidBlock CompressedtoCvBlock(Compressed comp);
CompVidBlock CompVidToBlock(ComponentVideo cv1,
                            ComponentVideo cv2,
                            ComponentVideo cv3,
                            ComponentVideo cv4);
signed quantize(float num);
float dequantize(signed num);
float inRange(float num, float min, float max);

#endif
