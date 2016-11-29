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


int main(int argc, char** argv) {

    DError* error = NULL;
    DImg* image = d_img_load_from_bmp_file("lenna_rgba.bmp",&error);
    
    if ( error )
        DLOG_ERR_E(error,"Cant load lenna_rgba.bmp");
    
    return (EXIT_SUCCESS);
}

