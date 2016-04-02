#include <stdlib.h>

#include "d_tools.h"

#define WARMUP_SIZE 25000000

void d_tools_warmup(){
    float *a,*b,*c;
    a = malloc(WARMUP_SIZE * sizeof(float));
    b = malloc(WARMUP_SIZE * sizeof(float));
    c = malloc(WARMUP_SIZE * sizeof(float));
    
    for(int i = 0; i < WARMUP_SIZE;i++){
        a[i] = rand() * 1000.0f / RAND_MAX;
        b[i] = rand() * 1000.0f / RAND_MAX;
    }
    
    for(int i = 0; i < WARMUP_SIZE;i++)
        c[i] = a[i] * b[i];
    
             
    free(a);
    free(b);
    free(c);
    
    
}

