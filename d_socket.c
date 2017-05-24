#include "d_socket.h"
#include "d_memory.h"

#include <sys/types.h>

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#define SOCKET_ERROR -1

#else
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#endif

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

typedef struct _d_socket {
    int socket_desc; // socket descriptor  

} DSocket;


#if !defined(__WIN32__)
DSocket* d_socket_connect_by_ip(char* ip, int port, DError** error) {

    DSocket* new_socket = d_malloc(sizeof (DSocket));


    struct sockaddr_in sock_adress;

    if ((sock_adress.sin_addr.s_addr = inet_addr(ip)) == -1) {
        if (error)
            *error = DERROR("IP adress %s is invalid", ip);
        goto error;
    };

    sock_adress.sin_family = AF_INET; /* Protocol IP */
    sock_adress.sin_port = htons(port);

    new_socket->socket_desc = socket(AF_INET, SOCK_STREAM, 0);


    if (new_socket->socket_desc == 0) {
        if (error)
            *error = DERROR("Cant create socket, %s", strerror(errno));
        goto error;
    }

    if (connect(new_socket->socket_desc, (const struct sockaddr*) &sock_adress, sizeof (sock_adress)) == SOCKET_ERROR) {
        if (error)
            *error = DERROR("Connection to %s:%d failed, %s", ip, port, strerror(errno));
        goto error;
    }

    return new_socket;

error:
    if (new_socket) d_socket_close(new_socket);
    return NULL;

}



void d_socket_close(DSocket* socket) {
    if (socket->socket_desc > 0) {

#if __WIN32__ == 1
        shutdown(socket->socket_desc, SHUT_RDWR);
#endif
        close(socket->socket_desc);
    }

    free(socket);

}

void d_socket_send(DSocket* socket, void* buffer, size_t len, DError** error) {

    int result = send(socket->socket_desc, buffer, len, 0);


    if (result == SOCKET_ERROR) {
        if (error)
            *error = DERROR("Error while sending data, %s", strerror(errno));
    }
}

#else


DSocket* d_socket_connect_by_ip(char* ip, int port, DError** error) {

    int iResult;
    WSADATA wsaData;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return NULL;
    }

    DSocket* new_socket = d_malloc(sizeof (DSocket));


    struct sockaddr_in sock_adress;

    if ((sock_adress.sin_addr.s_addr = inet_addr(ip)) == -1) {
        if (error)
            *error = DERROR("IP adress %s is invalid", ip);
        goto error;
    };

    sock_adress.sin_family = AF_INET; /* Protocol IP */
    sock_adress.sin_port = htons(port);

    new_socket->socket_desc = socket(AF_INET, SOCK_STREAM, 0);


    if (new_socket->socket_desc == INVALID_SOCKET) {
        if (error)
            *error = DERROR("Cant create socket, %s", strerror(errno));
        goto error;
    }

    if (connect(new_socket->socket_desc, (const struct sockaddr*) &sock_adress, sizeof (sock_adress)) == SOCKET_ERROR) {
        if (error)
            *error = DERROR("Connection to %s:%d failed, %s", ip, port, strerror(errno));
        goto error;
    }

    return new_socket;

error:
    if (new_socket) d_socket_close(new_socket);
    return NULL;

}

void d_socket_close(DSocket* socket) {
    if (socket->socket_desc > 0) {
        shutdown(socket->socket_desc, SD_BOTH);
        closesocket(socket->socket_desc);
    }
    free(socket);
}


void d_socket_send(DSocket* socket, void* buffer, size_t len, DError** error) {

    int result = send(socket->socket_desc, buffer, len, 0);

    if (result == SOCKET_ERROR) {
        if (error)
            *error = DERROR("Error while sending data, %s", strerror(errno));
    }
}

#endif