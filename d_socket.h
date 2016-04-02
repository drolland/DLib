
#include "d_error.h"
#include <stddef.h>

#pragma once

typedef struct _d_socket DSocket;

typedef struct _d_socket_adress {
    char* adresse;
    int port;
} DSocketAdress;


DSocket* d_socket_connect_by_ip(char* ip,int port,DError** error);

void d_socket_close(DSocket* socket);

void d_socket_send(DSocket* socket,void* buffer,size_t len,DError** error);