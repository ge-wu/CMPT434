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

int udp_talker(char*, char*);
int udp_listener();

int tcp_server();
int tcp_client(char*, char*);

int tcp_server_socket_gen(char*);
int tcp_client_socket_gen(char* , char*);

int udp_server_socket_gen(char*);
int udp_client_socket_gen(char*, char*);


#endif

