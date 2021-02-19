// Jiaye Wang jiw561 11231145

#ifndef _NETWORK_H
#define _NETWORK_H

#include <unistd.h>

#include <errno.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <netdb.h>

#define MSG_LEN 256
#define BACKLOG 10

#define PORT "30000"

int get_udp_server_socket(char * );

int get_udp_client_socket(char * , char * );


#endif