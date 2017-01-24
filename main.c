/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: drolland
 *
 * Created on 29 novembre 2016, 11:22
 */

#include <stdio.h>
#include <stdlib.h>
#include "d_error.h"
#include "d_img.h"
#include "d_logger.h"


char* input_file = "Lenna_rgba.bmp";
char* output_file_convolve = "Lenna_out_convolve.bmp";
char* output_file_bilinear = "Lenna_bilinear.bmp";
char* output_file_trilinear = "Lenna_trilinear.bmp";

int main(int argc, char** argv) {

    DError* error = NULL;
    
   
    DImg* image = d_img_load_from_bmp_file(input_file,&error);
     
    if ( error ){
        DLOG_ERR_E(error,"Cant load %s",input_file);
        d_error_free(&error);
    }
    
    printf("Convolve test\n");
    
    DImg* in_float = d_img_color_convert(image,DIMG_COLOR_FORMAT_RGBA_FLOAT,&error);
    
    float kernel_data[] = { 1.0f/8 , 1.0f/8, 1.0f/8, 1.0f/8 , 0 , 1.0f/8 , 1.0f/8 , 1.0f/8 , 1.0f/8};
    
    DKernel* kernel = d_kernel_new(kernel_data,3,3,DIMG_COLOR_FORMAT_RGBA_FLOAT);
    
    DImg* transformed_float = d_img_convolve(in_float,kernel);
        
    DImg* out_int = d_img_color_convert(transformed_float,DIMG_COLOR_FORMAT_RGBA,&error);
            
    if ( error ){
        DLOG_ERR_E(error,"can't convert from int to float");
        d_error_free(&error);
    }        
                 
    d_img_save_to_bmp_file(out_int,output_file_convolve,&error);
     
    if ( error ){
        DLOG_ERR_E(error,"Cant save %s",output_file_convolve);
        d_error_free(&error);
    }
    
    printf("Bilinear resize test\n");
    
    DImg* img_biliniear_resized = d_img_resize_bilinear(image,2048,2048);
    
    d_img_save_to_bmp_file(img_biliniear_resized,output_file_bilinear,&error);
    
    if ( error ){
        DLOG_ERR_E(error,"Cant save %s",output_file_convolve);
        d_error_free(&error);
    }
    
    printf("Trilinear resize test\n");
    
    DImg* img_triliniear_resized = d_img_resize_bicubic(image,2048,2048);
    
    d_img_save_to_bmp_file(img_triliniear_resized,output_file_trilinear,&error);
    
    if ( error ){
        DLOG_ERR_E(error,"Cant save %s",output_file_convolve);
        d_error_free(&error);
    }
    
    return (EXIT_SUCCESS);
}

