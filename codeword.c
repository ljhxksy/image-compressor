/**************************************************************
 *
 *                     codeword.c
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the implementation of functions for 
 *    packing components of a Compressed struct into a codeword and unpacking
 *    a codeword into the Compressed struct
 *
 **************************************************************/
#include "codeword.h"

/********** codeWord ********
 *
 * Creates a 64-bit word by packing components from a Compressed struct.
 *
 * Parameters:
 *      Compressed *comp: Pointer to a `Compressed` struct containing components
 *                        (a, b, c, d, pb_avg, pr_avg)
 *
 * Return:
 *      uint64_t: The resulting 64-bit word
 *
 * Expects:
 *      - comp must not be NULL
 *      - Memory allocated for comp will be freed
 * 
 ************************/
uint64_t codeWord(Compressed comp)
{
        assert(comp != NULL);
        uint64_t word = 0;

        word = Bitpack_newu(word, 6, 26, comp->a);
        word = Bitpack_news(word, 6, 20, comp->b);
        word = Bitpack_news(word, 6, 14, comp->c);
        word = Bitpack_news(word, 6, 8, comp->d);
        word = Bitpack_newu(word, 4, 4, comp->pb_avg);
        word = Bitpack_newu(word, 4, 0, comp->pr_avg);

        FREE(comp);

        return word;
}

/********** getCodeWord ********
 * 
 * Retrieves a 64-bit codeword from a file.
 *
 * Parameters:
 *      FILE *input: A pointer to the input file stream.
 *
 * Return:
 *      uint64_t: The 64-bit codeword read from the file.
 *
 * Notes:
 *      - CRE if input is NULL
 * 
 *******************************/
uint64_t getCodeword(FILE *input)
{
        assert(input != NULL);
        uint64_t word = 0;

        for (int i = 24; i >= 0; i -= 8) {
                unsigned char c = getc(input);
                /* Reads 8 bytes (64 bits) from the file */
                word = Bitpack_newu(word, 8, i, c);
        }

        return word;
}

/********** getCodeWord ********
 * 
 * Decodes a 64-bit word into a Compressed struct
 *
 * Parameters:
 *      uint64_t word: The 64-bit word to decode
 *
 * Return:
 *      Compressed: A Compressed struct containing the decoded values
 *
 * Notes:
 *      none
 * 
 *********************************/
Compressed decodeWord(uint64_t word)
{
        Compressed comp;
        NEW(comp);

        /* Extracts specific bit fields from the word using 
         * Bitpack functions and assign to fields in the Compressed struct
         */
        comp->a = Bitpack_getu(word, 6, 26);
        comp->b = Bitpack_gets(word, 6, 20);
        comp->c = Bitpack_gets(word, 6, 14);
        comp->d = Bitpack_gets(word, 6, 8);
        comp->pb_avg = Bitpack_getu(word, 4, 4);
        comp->pr_avg = Bitpack_getu(word, 4, 0);

        return comp;
}

/********** writeCompressed ********
 * 
 * Writes a 64-bit word to standard output in big-endian order
 *
 * Parameters:
 *      uint64_t word: The 64-bit word to write
 * 
 * Return: none
 *
 * Notes: none
 * 
 *********************************/
void writeCompressed(uint64_t word)
{
        for (int i = 24; i >= 0; i -= 8) {
                putchar(Bitpack_getu(word, 8, i));
        }
}