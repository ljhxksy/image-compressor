/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *     The file provides functions for compressing and decompressing PPM images
 *     given from the input file
 *
 **************************************************************/
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
#include "compress40.h"
#include "helpers.h"

/********** compress40 ********
 * 
 * Compresses a PPM image given from the input file
 *
 * Parameters:
 *      FILE *input: A pointer to the input file stream 
 *                   containing the PPM image.
 * 
 * Return: none
 *
 * Notes:  none
 *      
 **********************************/
extern void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods != NULL);
        Pnm_ppm image = Pnm_ppmread(input, methods);
        
        /* Calculates and trim the width and height of the compressed image */
        int height = image->height - (1 & image->height);
        int width = image->width - (1 & image->width);

        fprintf(stdout, "COMP40 Compressed image format 2\n%u %u\n", 
                width, height);

        /* Processes each 2x2 block of pixels in the image */
        for (int row = 0; row < height; row += 2) {
                for (int col = 0; col < width; col += 2) {
                        rgbBlock rgbBlock = imageToRgbBlock(image, col, row);
                        CompVidBlock cvBlock = rgbBlockToCvBlock(rgbBlock);
                        Compressed comp = DCT(cvBlock->cv1, cvBlock->cv2, 
                                              cvBlock->cv3, cvBlock->cv4);
                        uint64_t word = codeWord(comp);
                        writeCompressed(word);

                        freeCompression(rgbBlock, cvBlock);    
                }
        }

        Pnm_ppmfree(&image);
}

/********** decompress40 ********
 * 
 * Decompresses a compressed PPM image given from the input file
 *
 * Parameters:
 *      FILE *input: A pointer to the input file stream 
 *                   containing the compressed PPM image.
 * 
 * Return: none
 *
 * Notes:  none
 *      
 **********************************/
extern void decompress40(FILE *input)
{
        Pnm_ppm pixmap = readHeader(input);
        uint64_t countBlocks = 0;

        for (unsigned row = 0; row < pixmap->height; row += 2) {
                for (unsigned col = 0; col < pixmap->width; col += 2) {
                        uint64_t word = getCodeword(input);
                        Compressed comp = decodeWord(word);
                        CompVidBlock cvBlock = CompressedtoCvBlock(comp);
                        rgbBlock rgbBlock = CompVidBlockToRgbBlock(cvBlock);
                        RGBtoImage(pixmap, rgbBlock->rgb1, col, row);
                        RGBtoImage(pixmap, rgbBlock->rgb2, col + 1, row);
                        RGBtoImage(pixmap, rgbBlock->rgb3, col, row + 1);
                        RGBtoImage(pixmap, rgbBlock->rgb4, col + 1, row + 1);

                        countBlocks++;

                        freeDecompression(comp, cvBlock, rgbBlock);
                }
        }

        /* Count blocks to make sure that the amount of pixels fit the
         * dimensions of the image */
        if (countBlocks < ((pixmap->width * pixmap->height) / 4)) {
                exit(EXIT_FAILURE);
        }
        
        Pnm_ppmwrite(stdout, pixmap);
        Pnm_ppmfree(&pixmap);
}
