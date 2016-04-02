#pragma once

#include <stdlib.h>
#include "d_types.h"

void* d_malloc(size_t size);

void  d_set_malloc_exit_on_failure(boolean b);

void* d_mpool_alloc(size_t size);

void d_mpool_release();

void d_mpool_free();

void d_memory_print_memusage();