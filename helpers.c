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

/********** freeCompression ********
 * 
 * Frees memory associated with a rgbBlock and a CompVidBlock
 *
 * Parameters:
 *      rgbBlock rgbBlock: Pointer to the rgbBlock struct
 *      CompVidBlock cvBlock: Pointer to the CompVidBlock struct
 * 
 * Return: none
 *
 * Notes:
 *      - CRE if rgbBlock or cvBlock is NULL
 * 
 **********************************/
void freeCompression(rgbBlock rgbBlock, CompVidBlock cvBlock)
{
        assert(rgbBlock != NULL && cvBlock != NULL);

        /* Frees memory for RGB components */
        FREE(rgbBlock->rgb1);
        FREE(rgbBlock->rgb2);
        FREE(rgbBlock->rgb3);
        FREE(rgbBlock->rgb4);
        FREE(rgbBlock); /* Frees memory for the RGB block */

        /* Frees memory for compressed video components */
        FREE(cvBlock->cv1);
        FREE(cvBlock->cv2);
        FREE(cvBlock->cv3);
        FREE(cvBlock->cv4);
        FREE(cvBlock); /* Frees memory for the Component Video block */
}

/********** freeDecompression ********
 * 
 * Frees memory associated with a rgbBlock, a CompVidBlock, and 
 * a Compressed struct
 *
 * Parameters:
 *      rgbBlock rgbBlock: Pointer to the rgbBlock struct
 *      CompVidBlock cvBlock: Pointer to the CompVidBlock struct
 *      Compressed comp: Pointer to the Compressed struct
 * 
 * Return: none
 *
 * Notes:
 *      - CRE if rgbBlock or cvBlock is NULL
 * 
 **********************************/
void freeDecompression(Compressed comp, CompVidBlock cvBlock, rgbBlock rgbBlock)
{
        assert(comp != NULL && cvBlock != NULL && rgbBlock != NULL);
        FREE(comp);

        FREE(cvBlock->cv1);
        FREE(cvBlock->cv2);
        FREE(cvBlock->cv3);
        FREE(cvBlock->cv4);
        FREE(cvBlock);

        FREE(rgbBlock->rgb1);
        FREE(rgbBlock->rgb2);
        FREE(rgbBlock->rgb3);
        FREE(rgbBlock->rgb4);
        FREE(rgbBlock);
}

/********** readHeader ********
 * 
 * Reads the header of a compressed PPM file
 *
 * Parameters:
 *      FILE *input: A pointer to the input file stream containing 
 *                   the PPM image.
 *
 * Return:
 *      Pnm_ppm: A pointer to a Pnm_ppm struct 
 *
 * Notes:
 *      none
 * 
 ******************************/
Pnm_ppm readHeader(FILE *input)
{
        Pnm_ppm pixmap;
        NEW(pixmap);   

        A2Methods_T methods = uarray2_methods_plain;
        assert(methods != NULL);

        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');
        
        pixmap->width = width;
        pixmap->height = height;
        pixmap->denominator = 255; 
        pixmap->pixels = methods->new(width, height, sizeof(struct Pnm_rgb));
        pixmap->methods = methods;

        return pixmap;
}
