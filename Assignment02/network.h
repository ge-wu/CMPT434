// Jiaye Wang jiw561 11231145

#ifndef _NETWORK_H
#define _NETWORK_H

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "30000"

#define MIN_TIME 1
#define MAX_TIME 30

#define MIN_WSIZE 1
#define MAX_WSIZE 7

typedef unsigned int seq_nr;
typedef enum {data, ack, nak} frame_kind;
typedef enum {false, true} boolean;
typedef struct { unsigned char data[MAX_WSIZE]; } packet;
typedef struct {
  seq_nr seq; 
  char msg[128];
} frame;


// generate a UDP socket for server (receiver) 
// param: the port number
int get_udp_server_socket(char * );

// generate a UDP socket for client (sender)
// param: the hostname and the port number
int get_udp_client_socket(char * , char * );


#endif
