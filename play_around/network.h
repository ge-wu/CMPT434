#ifndef _NETWORK_H
#define _NETWORK_H

#include <unistd.h>

#include <errno.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <netdb.h>

#define MSG_LEN 1024
#define BACKLOG 10

#define PROXY_PORT "30001"
#define TCP_PORT "30002"
#define UDP_PORT "30003"

int get_udp_client_socket();

int get_udp_server_socket();

int get_tcp_server_socket();

int get_tcp_client_socket(char * , char * );

#endif