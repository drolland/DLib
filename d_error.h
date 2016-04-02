#pragma once

enum {
    ERROR_LEVEL_WARNING,
    ERROR_LEVEL_ERROR,
    ERROR_LEVEL_CRITICAL,
    ERROR_LEVEL_FATAL
};

typedef struct _derror {
    int level;
    char* file;
    int line;
    char* msg;
} DError;


DError* d_error_new(int level,const char* file,int line,const char* msg,...);

void d_error_free(DError* error);

/* Fast Macro*/

#define DERROR(msg,...) d_error_new(ERROR_LEVEL_ERROR, __FILE__, __LINE__, msg,##__VA_ARGS__)

enum {
    DERROR_UNKNOW_ERROR,
    DERROR_IO_ERROR,
    DERROR_FILE_OPEN,
    DERROR_UNSUPPORTED_FORMAT,
    DERROR_BUFFER_OVERFLOW,
    
};



