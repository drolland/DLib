#pragma once

#include "d_error.h"

enum {
    DIMG_COLOR_FORMAT_RGB = 3,
    DIMG_COLOR_FORMAT_RGBA = 4,
    DIMG_COLOR_FORMAT_RGB_FLOAT = 12,
    DIMG_COLOR_FORMAT_RGBA_FLOAT = 16
};

typedef struct _dimg {
    int color_format;
    int width;
    int height;
    int pitch;
    unsigned char* pixels;
} DImg;

typedef struct _dkernel {
    float* data;
    int color_format;
    int nb_colors;
    int pixel_width;
    int pixel_height;
} DKernel;

/* Convenient pixels acces macro*/

#define PIXEL_R(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch]
#define PIXEL_G(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch+1]
#define PIXEL_B(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch+2]
#define PIXEL_A(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch+3]

#define PIXEL_R_F(img,x,y) ((float*)img->pixels)[(x)*img->color_format/4+(y)*img->pitch/4]
#define PIXEL_G_F(img,x,y) ((float*)img->pixels)[(x)*img->color_format/4+(y)*img->pitch/4+1]
#define PIXEL_B_F(img,x,y) ((float*)img->pixels)[(x)*img->color_format/4+(y)*img->pitch/4+2]
#define PIXEL_A_F(img,x,y) ((float*)img->pixels)[(x)*img->color_format/4+(y)*img->pitch/4+3]

DImg* d_img_new_image(int width,int height,int color_format);

void d_img_free(DImg* image);

DImg* d_img_load_from_bmp_file(char* filepath,DError** error);

void d_img_save_to_bmp_file(DImg* img,char* filepath, DError** error);

DImg* d_img_resize_bilinear(DImg* img,int width,int height);

DImg* d_img_resize_bicubic(DImg* img,int width,int height);

DImg* d_img_color_convert(DImg* img,int new_color_format,DError** error);

DKernel* d_kernel_new(float* data,int pixel_width,int pixel_height,int color_format);

DImg* d_img_convolve(DImg* self,DKernel* kernel);