/**************************************************************
 *
 *                     helpers.h
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the declaration of helper functions and structs 
 *    that are used across the compression and decompression of ppm images
 *
 **************************************************************/
#ifndef HELPERS_INCLUDED
#define HELPERS_INCLUDED

#include <stdlib.h>
#include "arith40.h"

typedef struct ComponentVideo *ComponentVideo;
typedef struct CompVidBlock *CompVidBlock;
typedef struct Compressed *Compressed;
typedef struct RGB *RGB;
typedef struct rgbBlock *rgbBlock;

float inRange(float num, float min, float max);

struct ComponentVideo 
{
        float y, pb, pr;
};

struct CompVidBlock
{
        ComponentVideo cv1, cv2, cv3, cv4;
};

struct Compressed 
{
        unsigned pb_avg, pr_avg, a;
        signed b, c, d;
};

struct RGB 
{
        float r, g, b;
};

struct rgbBlock
{
        RGB rgb1, rgb2, rgb3, rgb4;
};

#endif