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
#define PORT "30000"

#define MIN_TIME 1
#define MAX_TIME 30

#define MIN_WSIZE 1
#define MAX_WSIZE 15

typedef struct FRAME {
  int sequence_num;
  char msg[MSG_LEN];
} FRAME;


// generate a UDP socket for server (receiver) 
// param: the port number
int get_udp_server_socket(char * );

// generate a UDP socket for client (sender)
// param: the hostname and the port number
int get_udp_client_socket(char * , char * );

// get sockaddr, IPv4 or IPv6 directly used from Beej's references. 
void * get_in_addr(struct sockaddr * );

#endif