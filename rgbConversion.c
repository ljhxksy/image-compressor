/**************************************************************
 *
 *                     rgbConversion.c
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the implementation of functions for converting RGB
 *    pixels in a Pnm image to RGB structs, then to Component Video structs.
 *    
 *
 **************************************************************/
#include "rgbConversion.h"

/********** imageToRgbBlock **********
 * 
 * Create an RGB block from a Pnm_ppm image at the specified column and row.
 *
 * Parameters:
 *      Pnm_ppm image: The input image.
 *      int col: The column index.
 *      int row: The row index.
 *
 * Return: An rgbBlock 
 *
 * Notes: 
 *      - rgbBlock will have to be freed manually outside the function
 * 
 **********************************/
rgbBlock imageToRgbBlock(Pnm_ppm image, int col, int row) 
{
        rgbBlock rgbBlock;
        NEW(rgbBlock);

        rgbBlock->rgb1 = imageToRGB(image, col, row);
        rgbBlock->rgb2 = imageToRGB(image, col + 1, row);
        rgbBlock->rgb3 = imageToRGB(image, col, row + 1);
        rgbBlock->rgb4 = imageToRGB(image, col + 1, row + 1);

        return rgbBlock;
}

/********** imageToRGB **********
 * 
 * Create an RGB struct from a Pnm_ppm image at the specified column and row.
 *
 * Parameters:
 *      Pnm_ppm image: The input image.
 *      int col: The column index.
 *      int row: The row index.
 *
 * Return: An RGB struct 
 *
 * Notes: 
 *      - RGB will be freed outside where the function is called
 * 
 *******************************/
RGB imageToRGB(Pnm_ppm image, int col, int row)
{
        RGB rgb;
        NEW(rgb);

        rgb->r = ((float)((Pnm_rgb)(image->methods
                ->at(image->pixels, col, row)))->red) / 255;
        rgb->g = ((float)((Pnm_rgb)(image->methods
                ->at(image->pixels, col, row)))->green) / 255; 
        rgb->b = ((float)((Pnm_rgb)(image->methods
                ->at(image->pixels, col, row)))->blue) / 255;

        return rgb;
}

/********** RGBtoImage **********
 * 
 * Update the RGB values of a Pnm_ppm image at the specified column and row.
 *
 * Parameters:
 *      Pnm_ppm image: The input image.
 *      RGB rgb: The RGB values to set.
 *      int col: The column index.
 *      int row: The row index.
 *
 * Notes:
 *      none
 * 
 *******************************/
void RGBtoImage(Pnm_ppm image, RGB rgb, int col, int row)
{
        assert(rgb != NULL);

        /* Scale RGB values from [0, 1] to [0, 255] */
        ((Pnm_rgb)image->methods->at(image->pixels, col, row))->red 
                                = (unsigned)inRange(rgb->r * 255, 0, 255);

        ((Pnm_rgb)image->methods->at(image->pixels, col, row))->green 
                                = (unsigned)inRange(rgb->g * 255, 0, 255);
        
        ((Pnm_rgb)image->methods->at(image->pixels, col, row))->blue 
                                = (unsigned)inRange(rgb->b * 255, 0, 255);
}

/********** compVidtoRGB **********
 * 
 * Convert ComponentVideo to RGB.
 *
 * Parameters:
 *      ComponentVideo cv: The input ComponentVideo.
 *
 * Return: An RGB struct 
 * 
 * Notes:
 *      - CRE if cv is NULL
 *      - The RGB struct is dynamically allocated and must be freed 
 *      by the caller.
 * 
 *****************************/
RGB compVidtoRGB(ComponentVideo cv)
{
        assert(cv != NULL);
        RGB rgb;
        NEW(rgb);

        rgb->r = 1.0 * cv->y + 0.0 * cv->pb + 1.402 * cv->pr;
        rgb->g = 1.0 * cv->y - 0.344136 * cv->pb - 0.714136 * cv->pr;
        rgb->b = 1.0 * cv->y + 1.772 * cv->pb + 0.081312 * cv->pr;

        return rgb;
}

/********** CompVidBlockToRgbBlock **********
 * 
 * Convert CompVidBLock to rgbBlock.
 *
 * Parameters:
 *      CompVidBlock cvBlock: The input ComponentVideo block.
 *
 * Return: An RGB block struct 
 * 
 * Notes:
 *      - CRE if cvBlock is NULL
 * 
 *****************************/
rgbBlock CompVidBlockToRgbBlock(CompVidBlock cvBlock)
{
        assert(cvBlock != NULL);

        RGB rgb1 = compVidtoRGB(cvBlock->cv1);
        RGB rgb2 = compVidtoRGB(cvBlock->cv2);
        RGB rgb3 = compVidtoRGB(cvBlock->cv3);
        RGB rgb4 = compVidtoRGB(cvBlock->cv4);
        
        return RGBtoRGBblock(rgb1, rgb2, rgb3, rgb4);
}

/********** RGBtoRGBblock **********
 * 
 * Convert 4 RGBs into an rgbBlock.
 *
 * Parameters:
 *      RGB rgb1, rgb2, rgb3, rgb4: The input RGB structs
 * 
 * Return: An RGB block struct 
 * 
 * Notes:
 *      - CRE if one of the RGB is NULL
 *      - The rgbBlock struct is dynamically allocated and must be freed 
 *      by the caller.
 * 
 *****************************/
rgbBlock RGBtoRGBblock(RGB rgb1, RGB rgb2, RGB rgb3, RGB rgb4)
{
        assert(rgb1 != NULL && rgb2 != NULL && rgb3 != NULL && rgb4 != NULL);
        rgbBlock rgbBlock;
        NEW(rgbBlock);

        rgbBlock->rgb1 = rgb1;
        rgbBlock->rgb2 = rgb2;
        rgbBlock->rgb3 = rgb3;
        rgbBlock->rgb4 = rgb4;

        return rgbBlock;
}