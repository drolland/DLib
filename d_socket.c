#include "d_socket.h"
#include "d_memory.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
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

DSocket* d_socket_connect_by_ip(char* ip,int port,DError** error) {

    
    struct sockaddr_in sock_adress;
    
    if ( (sock_adress.sin_addr.s_addr = inet_addr(ip)) == -1){
        if ( error)
            *error = DERROR("IP adress %s is invalid",ip);
        goto error;
    };
    
    sock_adress.sin_family = AF_INET; /* Protocol IP */
    sock_adress.sin_port = htons(port); 
    
    DSocket* new_socket = d_malloc(sizeof(DSocket));
    
    new_socket->socket_desc = socket(AF_INET,SOCK_STREAM,0);
    
/*  Not available on Linux, but kept there in case it would be needed by some other OSs/Platforms
    int set = 1;
    setsockopt(new_socket->socket_desc, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int);
*/
    
    if ( new_socket->socket_desc == -1){
        if ( error)
            *error = DERROR("Cant create socket, %s",strerror(errno));
        goto error;
    }
    
    if( connect(new_socket->socket_desc, (const struct sockaddr*)&sock_adress, sizeof(sock_adress)) == -1){
        if ( error )
            *error = DERROR("Connection to %s:%d failed",ip,port);
        goto error;
    }
    
/*
    struct linger so_linger;
    so_linger.l_onoff = TRUE;
    so_linger.l_linger = 30;
    setsockopt(new_socket->socket_desc,SOL_SOCKET,SO_LINGER,&so_linger,sizeof(so_linger));
*/

    return new_socket;
    
    error:
    if ( new_socket ) d_socket_close(new_socket);
    return NULL;
        
}

void d_socket_close(DSocket* socket){
    if ( socket->socket_desc > 0){
        shutdown(socket->socket_desc,SHUT_RDWR);
        close(socket->socket_desc);   
    }
    free(socket);
        
}

void d_socket_send(DSocket* socket,void* buffer,size_t len,DError** error){

    int result = send(socket->socket_desc,buffer,len,MSG_NOSIGNAL);

    if ( result == -1){
        if ( error )
            *error = DERROR("Error while sending data, %s",strerror(errno));
    }
}