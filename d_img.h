#pragma once

#include "d_error.h"

enum {
    DIMG_COLOR_FORMAT_RGB = 3,
    DIMG_COLOR_FORMAT_RGBA = 4
};

typedef struct _dimg {
    int color_format;
    int width;
    int height;
    int pitch;
    unsigned char* pixels;
} DImg;

/* Convenient pixels acces macro*/

#define PIXEL_R(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch]
#define PIXEL_G(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch+1]
#define PIXEL_B(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch+2]
#define PIXEL_A(img,x,y) img->pixels[(x)*img->color_format+(y)*img->pitch+3]

DImg* d_img_new_image(int width,int height,int color_format);

void d_img_free(DImg* image);

DImg* d_img_load_from_bmp_file(char* filepath,DError** error);

DImg* d_img_resize(DImg* img,int width,int height);