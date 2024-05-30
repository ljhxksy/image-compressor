/**************************************************************
 *
 *                     compVidConversion.c
 *
 *     Assignment: arith
 *     Authors: Anh Hoang, Emilio Aleman
 *     Date:  3/8/2024
 *
 *    This file contains the implementation of functions for converting
 *    Component Videos.
 *    
 *
 **************************************************************/
#include "compVidConversion.h"

/********** RGBtoCompVid **********
 * 
 * Converts an RGB struct to Component Video representation
 *
 * Parameters:
 *      RGB rgb: A pointer to an RGB struct
 *
 * Return:
 *      A pointer to a ComponentVideo struct
 *
 * Notes:
 *      - CRE if RGB is NULL
 *      - cv will be freed by user
 * 
 *********************************/
ComponentVideo RGBtoCompVid(RGB rgb) 
{
        assert(rgb != NULL);
        ComponentVideo cv;
        NEW(cv);
        
        cv->y = 0.299 * rgb->r + 0.587 * rgb->g + 0.114 * rgb->b;
        cv->pb = -0.168736 * rgb->r - 0.331264 * rgb->g + 0.5 * rgb->b;
        cv->pr = 0.5 * rgb->r - 0.418688 * rgb->g - 0.081312 * rgb->b;
        
        return cv;
}

/********** rgbBlockToCvBlock **********
 * 
 * Converts an RGB block to Component Video block
 *
 * Parameters:
 *      rgbBlock rgbBlock: A pointer to an rgbBlock struct
 *
 * Return:
 *      A pointer to a CompVidBlock struct
 *
 * Notes:
 *      - CRE if rgbBlock is NULL
 *      - cvBlock will be freed by user
 * 
 *********************************/
CompVidBlock rgbBlockToCvBlock(rgbBlock rgbBlock)
{
        assert(rgbBlock != NULL);
        CompVidBlock cvBlock;
        NEW(cvBlock);

        cvBlock->cv1 = RGBtoCompVid(rgbBlock->rgb1);
        cvBlock->cv2 = RGBtoCompVid(rgbBlock->rgb2);
        cvBlock->cv3 = RGBtoCompVid(rgbBlock->rgb3);
        cvBlock->cv4 = RGBtoCompVid(rgbBlock->rgb4);

        return cvBlock;
}

/********************* DCT *************************
 * 
 * Applies the Discrete Cosine Transform (DCT) to a set 
 * of Component Video values.
 *
 * Parameters:
 *      ComponentVideo cv1, cv2, cv3, cv4: A pointer to the first 
 *                                         Component Video structure.
 *
 * Return:
 *      A pointer to a Compressed struct 
 *
 * Notes:
 *      - CRE if any of the ComponentVideos is NULL
 *      - comp will be manually freed outside where the function is called
 * 
 **************************************************/
Compressed DCT(ComponentVideo cv1, ComponentVideo cv2, 
               ComponentVideo cv3, ComponentVideo cv4)
{
        assert(cv1 != NULL && cv2 != NULL && cv3 != NULL && cv4 != NULL);

        /* Calculates the average Pb and Pr values */
        float pb_avg = (cv1->pb + cv2->pb + cv3->pb + cv4->pb) / 4.0;
        float pr_avg = (cv1->pr + cv2->pr + cv3->pr + cv4->pr) / 4.0;

        /* Computes DCT coefficients (a, b, c, d) based on the Y values */
        float a = (cv4->y + cv3->y + cv2->y + cv1->y) / 4.0;
        float b = (cv4->y + cv3->y - cv2->y - cv1->y) / 4.0;
        float c = (cv4->y - cv3->y + cv2->y - cv1->y) / 4.0;
        float d = (cv4->y - cv3->y - cv2->y + cv1->y) / 4.0;

        Compressed comp;
        NEW(comp);
        
        /* Quantize the coefficients */
        comp->pb_avg = Arith40_index_of_chroma(pb_avg);
        comp->pr_avg = Arith40_index_of_chroma(pr_avg);

        comp->a = round(inRange(a, 0, 1) * 511);
        comp->b = quantize(b);
        comp->c = quantize(c);
        comp->d = quantize(d);

        return comp;  
}

/************* CompressedtoCvBlock **************
 * 
 * Converts a Compressed block to a ComponentVideo block.
 *
 * Parameters:
 *      Compressed comp: A pointer to a Compressed struct
 *
 * Returns:
 *      CompVidBlock: A ComponentVideo block resulting from the conversion.
 *
 * Notes:
 *      - CRE if comp is NULL
 *      - cv1-cv4 will be freed manually with cvBlock outside the function
 * 
 *************************************/
CompVidBlock CompressedtoCvBlock(Compressed comp)
{
        assert(comp != NULL);

        float a = (float)comp->a / 511.0;

        float b = dequantize((float)comp->b);
        float c = dequantize((float)comp->c);
        float d = dequantize((float)comp->d);

        float pb_avg = Arith40_chroma_of_index(comp->pb_avg);
        float pr_avg = Arith40_chroma_of_index(comp->pr_avg);

        ComponentVideo cv1, cv2, cv3, cv4;
        NEW(cv1);
        NEW(cv2);
        NEW(cv3);
        NEW(cv4);

        /* Apply dequantization */
        cv1->y = a - b - c + d;
        cv1->pb = pb_avg;
        cv1->pr = pr_avg;

        cv2->y = a - b + c - d;
        cv2->pb = pb_avg;
        cv2->pr = pr_avg;

        cv3->y = a + b - c - d;
        cv3->pb = pb_avg;
        cv3->pr = pr_avg;

        cv4->y = a + b + c + d;
        cv4->pb = pb_avg;
        cv4->pr = pr_avg;

        return CompVidToBlock(cv1, cv2, cv3, cv4);
}

/********** CompVidToBlock **********
 * 
 * Converts 4 Component Videos into a Component Video block
 *
 * Parameters:
 *      ComponentVide cv1, cv2, cv3, cv4: Pointers to a ComponentVideo struct
 *
 * Return:
 *      A pointer to a CompVidBlock struct
 *
 * Notes:
 *      - CRE if any of the ComponentVideos is NULL
 *      - cvBlock will be manually freed outside the function
 * 
 *********************************/
CompVidBlock CompVidToBlock(ComponentVideo cv1, ComponentVideo cv2, 
                            ComponentVideo cv3, ComponentVideo cv4)
{
        assert(cv1 != NULL && cv2 != NULL && cv3 != NULL && cv4 != NULL);

        CompVidBlock cvBlock;
        NEW(cvBlock);

        cvBlock->cv1 = cv1;
        cvBlock->cv2 = cv2;
        cvBlock->cv3 = cv3;
        cvBlock->cv4 = cv4;

        return cvBlock;
}

/********** quantize **********
 * 
 * Quantizes a floating-point number within a specified range.
 *
 * Parameters:
 *      float num: The input floating-point number to be quantized.
 *
 * Returns:
 *      signed: The quantized value as a signed integer.
 *
 * Notes: none
 * 
 ******************************/
signed quantize(float num)
{
        return (signed)(inRange(num, -0.3, 0.3) * 50);
}

/********** dequantize **********
 * 
 * Dequantizes a floating-point number within a specified range.
 *
 * Parameters:
 *      signed num: The input signed integer to be dequantized.
 *
 * Returns:
 *      float: The quantized value as a floating-point number
 *
 * Notes: none
 * 
 ******************************/
float dequantize(signed num)
{
        return inRange(num, -15, 15) / 50.0;
}

