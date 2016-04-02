
#define NO_DEBUG_LOG


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "d_img.h"
#include "d_memory.h"
#include "d_logger.h"


struct bitmap_header {
    char type[2];
    int32_t file_size;
    int16_t _reserved1; 
    int16_t _reserved2; 
    int32_t data_offset;
};

struct bitmap_info_header {
    int32_t header_size;
    int32_t width;
    int32_t height;
    int16_t nb_color_planes;
    int16_t bits_per_pixel;
    int32_t compression_method;
    int32_t raw_bitmap_data_size;
    int32_t horizontal_pixels_per_meter;
    int32_t vertical_pixels_per_meter;
    int32_t nb_colors_in_palette;
    int32_t nb_important_colors;
    /* V2 */
    uint32_t R_mask;
    uint32_t G_mask;
    uint32_t B_mask;
    /* V3 */
    uint32_t A_mask;
};

enum {
    BI_RGB = 0,
    BI_BITFIELD = 3,
    BI_ALPHABITFIELDS = 6
};

DImg* d_img_new_image(int width, int height, int color_format) {
    DImg* image = d_malloc(sizeof (DImg));
    image->width = width;
    image->height = height;
    image->color_format = color_format;
    image->pitch = ((width * color_format + 3) / 4) * 4 ;
    image->pixels = d_malloc(image->pitch * height * color_format * sizeof (char) );
    return image;
}

void d_img_free(DImg* image) {
    if (image->pixels) free(image->pixels);
    free(image);
}

DImg* d_img_load_from_bmp_file(char* filepath, DError** error) {

    DImg* new_image = NULL;
    char* conversion_buffer = NULL;
    
    FILE* fd = fopen(filepath, "rb");
    if (fd == NULL) {
        if (error != NULL)
            *error = DERROR(strerror(errno));
        goto error;
    }

    struct bitmap_header bh;

    int fread_count = fread(&bh, sizeof (struct bitmap_header), 1, fd);
    if (fread_count != 1) {
        if (error != NULL)
            *error = DERROR("IO error while reading bitmap header");
        goto error;
    }


    if (!(bh.type[0] == 'B' && bh.type[1] == 'M')) {
        if (error != NULL)
            *error = DERROR("Unsuported bitmap type ( Should be windows type )");
        goto error;
    }

    struct bitmap_info_header bih;

    fseek(fd, 14, SEEK_SET);
    fread_count = fread(&bih, sizeof (struct bitmap_info_header), 1, fd);
    if (fread_count != 1) {
        if (error != NULL)
            *error = DERROR("IO error while reading bitmap info header");
        goto error;
    }

    if (bih.compression_method != BI_RGB && bih.compression_method != BI_BITFIELD && bih.compression_method != BI_ALPHABITFIELDS) {
        if (error != NULL)
            *error = DERROR("Unsuported bitmap compression method");
        goto error;
    }

    if (bih.nb_colors_in_palette != 0) {
        if (error != NULL)
            *error = DERROR("bitmap color palette unsupported");
        goto error;
    }

    DLOGD("BIH header size : %d", bih.header_size);
    DLOGD("Compresion method : %d", bih.compression_method);
    DLOGD("Application codes : %d,%d", bh._reserved1, bh._reserved2);
    DLOGD("bits per pixel : %d", bih.bits_per_pixel);
    DLOGD("number important color : %d", bih.nb_important_colors);
    DLOGD("Number of colors in palette %d", bih.nb_colors_in_palette);
    DLOGD("R_Mask %08x", bih.R_mask);
    DLOGD("G_Mask %08x", bih.G_mask);
    DLOGD("B_Mask %08x", bih.B_mask);
    DLOGD("A_Mask %08x", bih.A_mask);

    if (bih.compression_method == BI_RGB) {
        if (bih.bits_per_pixel == 32) {
            bih.R_mask = 0xFF000000;
            bih.G_mask = 0x00FF0000;
            bih.B_mask = 0x0000FF00;
            bih.A_mask = 0x000000FF;
        } else if (bih.bits_per_pixel == 24) {
            bih.R_mask = 0x00FF0000;
            bih.G_mask = 0x0000FF00;
            bih.B_mask = 0x000000FF;
            bih.A_mask = 0;
        } else if (bih.bits_per_pixel == 16) {
            if (error != NULL)
                *error = DERROR("Bitmap file invalid : compression method is BI_RGB but masks must be defined for 16bits");
            goto error;
        }

    }

    DLOGD("R_Mask %08x", bih.R_mask);
    DLOGD("G_Mask %08x", bih.G_mask);
    DLOGD("B_Mask %08x", bih.B_mask);
    DLOGD("A_Mask %08x", bih.A_mask);

    int R_mask_bit_shift = 0;
    int R_bit_depth = 0;
    float R_scale_factor = 1;
    int G_mask_bit_shift = 0;
    int G_bit_depth = 0;
    float G_scale_factor = 1;
    int B_mask_bit_shift = 0;
    int B_bit_depth = 0;
    float B_scale_factor = 1;
    int A_mask_bit_shift = 0;
    int A_bit_depth = 0;
    float A_scale_factor = 1;

    /* Determine bit shifth and bit depth for each color */
    int k = 0;
    for (; k < 31; k++) {
        if ((bih.R_mask & 1 << k) != 0) {
            R_mask_bit_shift = k;
            break;
        }
    }

    if (k == 31)
        R_bit_depth = 0;
    else
        for (; k < 33; k++) {
            if ((bih.R_mask & 1 << k) == 0) {
                R_bit_depth = k - R_mask_bit_shift;
                break;
            }
        }
    R_scale_factor = 255.0f / (pow(2, R_bit_depth) - 1);

    k = 0;
    for (; k < 31; k++) {
        if ((bih.G_mask & 1 << k) != 0) {
            G_mask_bit_shift = k;
            break;
        }
    }

    if (k == 31)
        G_bit_depth = 0;
    else
        for (; k < 33; k++) {
            if ((bih.G_mask & 1 << k) == 0) {
                G_bit_depth = k - G_mask_bit_shift;
                break;
            }
        }
    G_scale_factor = 255.0f / (pow(2, G_bit_depth) - 1);

    k = 0;
    for (; k < 31; k++) {
        if ((bih.B_mask & 1 << k) != 0) {
            B_mask_bit_shift = k;
            break;
        }
    }

    if (k == 31)
        B_bit_depth = 0;
    else
        for (; k < 33; k++) {
            if ((bih.B_mask & 1 << k) == 0) {
                B_bit_depth = k - B_mask_bit_shift;
                break;
            }
        }
    B_scale_factor = 255.0f / (pow(2, B_bit_depth) - 1);

    k = 0;
    for (; k < 31; k++) {
        if ((bih.A_mask & 1 << k) != 0) {
            A_mask_bit_shift = k;
            break;
        }
    }

    if (k == 31)
        A_bit_depth = 0;
    else
        for (; k < 33; k++) {
            if ((bih.A_mask & 1 << k) == 0) {
                A_bit_depth = k - A_mask_bit_shift;
                break;
            }
        }
    A_scale_factor = 255.0f / (pow(2, A_bit_depth) - 1);

    DLOGD("Red bit shift : %d; bit depth %d", R_mask_bit_shift, R_bit_depth);
    DLOGD("Green bit shift : %d; bit depth %d", G_mask_bit_shift, G_bit_depth);
    DLOGD("Blue bit shift : %d; bit depth %d", B_mask_bit_shift, B_bit_depth);
    DLOGD("Alpha bit shift : %d; bit depth %d", A_mask_bit_shift, A_bit_depth);



    /* Load pixels data*/

    int data_offset = 14 + bih.header_size;
    fseek(fd, data_offset, SEEK_SET);

    

    if (bih.bits_per_pixel == 32) {

        new_image = d_img_new_image(bih.width, bih.height, DIMG_COLOR_FORMAT_RGBA);

        conversion_buffer = d_malloc(new_image->pitch * sizeof (char));
        
        for (int h = 0; h < new_image->height; h++) {
            if (fread(conversion_buffer, new_image->pitch * sizeof (char), 1, fd) == 0) {
                if (error != NULL) {
                    *error = DERROR("IO Error while reading pixels data");
                    goto error;
                }
            }

            for (int w = 0; w < new_image->width; w++) {
                int32_t* pixel = (int32_t*) (conversion_buffer + 4 * w);
                PIXEL_R(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.R_mask) >> R_mask_bit_shift) * R_scale_factor;
                PIXEL_G(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.G_mask) >> G_mask_bit_shift) * G_scale_factor;
                PIXEL_B(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.B_mask) >> B_mask_bit_shift) * B_scale_factor;
                PIXEL_A(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.A_mask) >> A_mask_bit_shift) * A_scale_factor;
            }
        }


    } else if (bih.bits_per_pixel == 24) {

        new_image = d_img_new_image(bih.width, bih.height, DIMG_COLOR_FORMAT_RGB);

        conversion_buffer = d_malloc(new_image->pitch * sizeof (char));
        
        for (int h = 0; h < new_image->height; h++) {
            if (fread(conversion_buffer, new_image->pitch * sizeof (char), 1, fd) == 0) {
                if (error != NULL) {
                    *error = DERROR("IO Error while reading pixels data");
                    goto error;
                }
            }

            for (int w = 0; w < new_image->width; w++) {
                int32_t* pixel = (int32_t*) (conversion_buffer + 3 * w);
                PIXEL_R(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.R_mask) >> R_mask_bit_shift) * R_scale_factor;
                PIXEL_G(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.G_mask) >> G_mask_bit_shift) * G_scale_factor;
                PIXEL_B(new_image, w, new_image->height - 1 - h) = ((*pixel & bih.B_mask) >> B_mask_bit_shift) * B_scale_factor;
                
            }
        }

    } else if (bih.bits_per_pixel == 16) {
        
        if (A_bit_depth > 0) {
            new_image = d_img_new_image(bih.width, bih.height, DIMG_COLOR_FORMAT_RGBA);
        } else {
            new_image = d_img_new_image(bih.width, bih.height, DIMG_COLOR_FORMAT_RGB);
        }

        int pitch = ((new_image->width * 2 + 3) / 4) * 4;
        conversion_buffer = d_malloc(pitch * sizeof (char));

        for (int h = 0; h < new_image->height; h++) {
            if (fread(conversion_buffer, pitch * sizeof (char), 1, fd) == 0) {
                if (error != NULL) {
                    *error = DERROR("IO Error while reading pixels data");
                    goto error;
                }
            }
            for (int w = 0; w < new_image->width; w++) {
                PIXEL_R(new_image, w, new_image->height - 1 - h) = ((*(((int16_t*) conversion_buffer) + w) & bih.R_mask) >> R_mask_bit_shift) * R_scale_factor;
                PIXEL_G(new_image, w, new_image->height - 1 - h) = ((*(((int16_t*) conversion_buffer) + w) & bih.G_mask) >> G_mask_bit_shift) * G_scale_factor;
                PIXEL_B(new_image, w, new_image->height - 1 - h) = ((*(((int16_t*) conversion_buffer) + w) & bih.B_mask) >> B_mask_bit_shift) * B_scale_factor;
                if (A_bit_depth > 0)
                    PIXEL_A(new_image, w, new_image->height - 1 - h) = ((*(((int16_t*) conversion_buffer) + w) & bih.A_mask) >> A_mask_bit_shift) * A_scale_factor;
            }
        }
    } else {
        if (error != NULL)
            *error = DERROR("Unsuported bits depth");
        goto error;
    }


    if ( fd ) fclose(fd);
    if (conversion_buffer) free(conversion_buffer);
    return new_image;

error:
    if ( fd ) fclose(fd);
    if (conversion_buffer) free(conversion_buffer);
    if (new_image) d_img_free(new_image);
    return NULL;

}

enum {
    RED,
    GREEN,
    BLUE,
    ALPHA
};

static inline unsigned char get_pixel_clamped(DImg* img, int color, int x, int y) {
    if (x > 0)
        if (x < img->width)
            if (y > 0)
                if (y < img->height)
                    return img->pixels[x * img->color_format + y * img->pitch + color];
                else
                    return img->pixels[x * img->color_format + (img->height - 1) * img->pitch + color];
            else
                return img->pixels[x * img->color_format + 0 * img->pitch + color];
        else
            if (y > 0)
            if (y < img->height)
                return img->pixels[(img->width - 1) * img->color_format + y * img->pitch + color];
            else
                return img->pixels[(img->width - 1) * img->color_format + (img->height - 1) * img->pitch + color];
        else
            return img->pixels[(img->width - 1) * img->color_format + 0 * img->pitch + color];
    else
        if (y > 0)
        if (y < img->height)
            return img->pixels[0 * img->color_format + y * img->pitch + color];
        else
            return img->pixels[0 * img->color_format + (img->height - 1) * img->pitch + color];
    else
        return img->pixels[color];
}

static inline unsigned char get_interpolated_pixel_bilinear(DImg* img, int color, float x, float y) {
    int rounded_x_1 = x;
    int rounded_x_2 = x + 1;
    int rounded_y_1 = y;
    int rounded_y_2 = y + 1;
    float ex = x - rounded_x_1;
    float ey = y - rounded_y_1;
    float interpol_x_1 = (1 - ex) * get_pixel_clamped(img, color, rounded_x_1, rounded_y_1) + (ex) * get_pixel_clamped(img, color, rounded_x_2, rounded_y_1);
    float interpol_x_2 = (1 - ex) * get_pixel_clamped(img, color, rounded_x_1, rounded_y_2) + (ex) * get_pixel_clamped(img, color, rounded_x_2, rounded_y_2);
    return ( 1.0f - ey) * interpol_x_1 + (ey) * interpol_x_2;
}

DImg* d_img_resize(DImg* img, int width, int height) {

    DImg* new_image = d_img_new_image(width, height, img->color_format);

    float scale_factor_x = (float) img->width / (float) width;
    float scale_factor_y = (float) img->height / (float) height;

    if (img->color_format == DIMG_COLOR_FORMAT_RGBA) {
        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++) {
                PIXEL_R(new_image, i, j) = get_interpolated_pixel_bilinear(img, RED, (float) i*scale_factor_x, (float) j * scale_factor_y);
                PIXEL_G(new_image, i, j) = get_interpolated_pixel_bilinear(img, GREEN, (float) i*scale_factor_x, (float) j * scale_factor_y);
                PIXEL_B(new_image, i, j) = get_interpolated_pixel_bilinear(img, BLUE, (float) i*scale_factor_x, (float) j * scale_factor_y);
                PIXEL_A(new_image, i, j) = get_interpolated_pixel_bilinear(img, ALPHA, (float) i*scale_factor_x, (float) j * scale_factor_y);
            }
    } else {
        for (int j = 0; j < height; j++)
            for (int i = 0; i < width; i++) {
                PIXEL_R(new_image, i, j) = get_interpolated_pixel_bilinear(img, RED, (float) i*scale_factor_x, (float) j * scale_factor_y);
                PIXEL_G(new_image, i, j) = get_interpolated_pixel_bilinear(img, GREEN, (float) i*scale_factor_x, (float) j * scale_factor_y);
                PIXEL_B(new_image, i, j) = get_interpolated_pixel_bilinear(img, BLUE, (float) i*scale_factor_x, (float) j * scale_factor_y);
            }
    }

    return new_image;
}