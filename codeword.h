/**************************************************************
 *
 *                     codeword.h
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the definition of functions for 
 *    packing components of a Compressed struct into a codeword and unpacking
 *    a codeword into the Compressed struct
 *
 **************************************************************/
#ifndef CODEWORD_INCLUDED
#define CODEWORD_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mem.h"
#include "assert.h"
#include "except.h"
#include "pnm.h"
#include "bitpack.h"
#include "helpers.h"

uint64_t codeWord(Compressed comp);
uint64_t getCodeword(FILE *input);
Compressed decodeWord(uint64_t word);
void writeCompressed(uint64_t word);

#endif
