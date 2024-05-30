/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the implementation of functions for 
 *    manipulating bit-packed data.
 *
 **************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "except.h"
#include "bitpack.h"

Except_T Bitpack_Overflow = {"Overflow packing bits"};

/********** shiftUnsigned ********
 *
 * Shifts an unsigned 64-bit value either to the right or left.
 *
 * Parameters:
 *      uint64_t val:       The value to be shifted
 *      unsigned shift:     The number of bits to shift (0 to 63)
 *      bool right:         1 for right shift, 0 for left shift
 *
 * Return:
 *      uint64_t: The result of the shift operation
 *
 * Expects:
 *      - shift must be in the range [0, 63]
 *      - If shift is 64, the function returns 0
 ************************/
uint64_t shiftUnsigned(uint64_t val, unsigned shift, bool right)
{       
        if (shift == (unsigned) 64) {
                return 0;
        }
        if (right) {
                return ((uint64_t)val >> shift);
        }
        return ((uint64_t)val << shift);
}

/********** rightShiftSigned ********
 *
 * Right-shifts a signed 64-bit value.
 *
 * Parameters:
 *      int64_t val:       The value to be shifted
 *      unsigned shift:    The number of bits to shift (0 to 63)
 *
 * Return:
 *      int64_t: The result of the right shift operation
 *
 * Expects:
 *      - shift must be in the range [0, 63]
 *      - If shift is 64, the function returns 0
 ************************/
int64_t rightShiftSigned(int64_t val, unsigned shift) 
{
        if (shift == (unsigned) 64) {
                return 0;
        }
        return ((int64_t)val >> shift);
}

/********** leftShiftSigned ********
 *
 * Left-shifts a signed 64-bit value.
 *
 * Parameters:
 *      int64_t val:       The value to be shifted
 *      unsigned shift:    The number of bits to shift (0 to 63)
 *
 * Return:
 *      int64_t: The result of the right shift operation
 *
 * Expects:
 *      - shift must be in the range [0, 63]
 *      - If shift is 64, the function returns 0
 ************************/
int64_t leftShiftSigned(int64_t val, unsigned shift)
{
        if (shift == (unsigned) 64) {
                return 0;
        }
        return ((int64_t)val << shift);
}

/********** Bitpack_fitsu ********
 *
 * Checks if a given unsigned integer can fit into a specified width.
 *
 * Parameters:
 *      uint64_t n:     the unsigned integer to check.
 *      unsigned width: the width to check against.
 *
 * Return: true if n fits into width, false if not.
 *
 * Expects:
 *      width must be less than or equal to 64.
 * Notes:
 *      Will CRE if width is greater than 64.
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= (unsigned) 64);
        
        if (n < shiftUnsigned(1, width, false)) {
                return true;
        }
        return false;
}

/********** Bitpack_fitss ********
 *
 * Checks if a given signed integer can fit into a specified width.
 *
 * Parameters:
 *      int64_t n:      the signed integer to check.
 *      unsigned width: the width to check against.
 *
 * Return: true if n fits into width, false if not.
 *
 * Expects:
 *      width must be less than or equal to 64.
 * Notes:
 *      Will CRE if width is greater than 64.
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= (unsigned) 64);

        int64_t max = leftShiftSigned(1, (width - 1)) - 1;
        int64_t min = ~max;

        return !(n > max || n < min);
}

/********** createMask ********
 *
 * Creates a 64-bit mask with a specified shift.
 *
 * Parameters:
 *      uint64_t shift: the shift to create the mask with.
 *
 * Return: the created mask.
 * 
 * Expects:
 *      shift must be greater than or equal to 0.
 * Notes:
 *      Will CRE if shift is less than 0.
 ************************/
uint64_t createMask(uint64_t shift)
{
        return shiftUnsigned(1, shift, false) - 1;
}

/********** Bitpack_getu ********
 *
 * Gets an unsigned integer from a word at a specified position and width.
 *
 * Parameters:
 *      uint64_t word:  the word to get the integer from.
 *      unsigned width: width of the integer.
 *      unsigned lsb:   the least significant bit of the integer in the word.
 *
 * Return: the unsigned integer.
 *
 * Expects:
 *      width must be less than or equal to 64.
 *      width + lsb must be less than or equal to 64.
 * Notes:
 *      Will CRE if width or width + lsb are greater than 64.
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= (unsigned) 64);
        assert((width + lsb) <= 64);

        /* create a mask, bit-wise & together, shift right */
        word = shiftUnsigned(word, lsb, true);
        uint64_t mask = createMask(width);

        return (word & mask);
}

/********** Bitpack_gets ********
 *
 * Gets a signed integer from a word at a specified position and width.
 *
 * Parameters:
 *      uint64_t word:  the word to get the integer from.
 *      unsigned width: width of the integer.
 *      unsigned lsb:   the least significant bit of the integer in the word.
 *
 * Return: the signed integer.
 *
 * Expects:
 *      width must be less than or equal to 64.
 *      width + lsb must be less than or equal to 64.
 * Notes:
 *      Will CRE if width or width + lsb are greater than 64.
 ************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= (unsigned) 64);
        assert((width + lsb) <= 64);

        word = shiftUnsigned(word, lsb, true);
        uint64_t mask = createMask(width);

        int64_t result = shiftUnsigned(word & mask, 64 - width, false);

        return rightShiftSigned(result, 64 - width);
}

/********** Bitpack_newu ********
 *
 * Inserts an unsigned integer into a word at a specified position and width.
 *
 * Parameters:
 *      uint64_t word:  the word to insert the integer into.
 *      unsigned width: width of the integer.
 *      unsigned lsb:   the least significant bit of the integer.
 *      uint64_t value: the unsigned integer to insert.
 *
 * Return: the new word.
 *
 * Expects:
 *      width must be less than or equal to 64.
 *      width + lsb must be less than or equal to 64.
 *      value must fit into width.
 * Notes:
 *      Will CRE if width or width + lsb are greater than 64.
 *      Will raise exception if value doesn't fit in width.
 ************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, 
                      unsigned lsb, uint64_t value)
{
        assert(width <= (unsigned) 64);
        assert((width + lsb) <= 64);

        if (!Bitpack_fitsu(value, width)) {
                fprintf(stderr, "width: %u, val: %lu\n", width, value);
                RAISE(Bitpack_Overflow);
        }

        uint64_t flippedMask = ~(shiftUnsigned(createMask(width), lsb, false));
        word = word & flippedMask;

        uint64_t newField = shiftUnsigned(value, lsb, false);
        
        return word | newField;
}

/********** Bitpack_news ********
 *
 * Inserts a signed integer into a word at a specified position and width.
 *
 * Parameters:
 *      uint64_t word:  the word to insert the integer into.
 *      unsigned width: width of the integer.
 *      unsigned lsb:   the least significant bit of the integer.
 *      int64_t value:  the signed integer to insert.
 *
 * Return: the new word.
 *
 * Expects:
 *      width must be less than or equal to 64.
 *      width + lsb must be less than or equal to 64.
 *      value must fit into width.
 * Notes:
 *      Will CRE if width or width + lsb are greater than 64.
 *      Will raise exception if value doesn't fit in width.
 ************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, 
                      unsigned lsb, int64_t value)
{
        assert(width <= (unsigned) 64);
        assert((width + lsb) <= 64);

        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        value = Bitpack_getu(value, width, 0);

        return Bitpack_newu(word, width, lsb, value);
}
