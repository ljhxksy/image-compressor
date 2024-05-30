/**************************************************************
 *
 *                     compress40.h
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *     The file provides function declarations for compressing and 
 *     decompressing PPM images given from the input file
 *
 **************************************************************/
#ifndef COMPRESS40_INCLUDED
#define COMPRESS40_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assert.h"
#include "except.h"
#include "mem.h"
#include "pnm.h"
#include "bitpack.h"
#include "a2plain.h"
#include "a2methods.h"
#include "arith40.h"

#include "helpers.h"
#include "rgbConversion.h"
#include "codeword.h"
#include "compVidConversion.h"

/*
 * The two functions below are functions you should implement.
 * They should take their input from the parameter and should
 *  write their output to stdout
 */
extern void compress40  (FILE *input);  /* reads PPM, writes compressed image */
extern void decompress40(FILE *input);  /* reads compressed image, writes PPM */

#endif
