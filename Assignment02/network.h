// Jiaye Wang jiw561 11231145

#ifndef _NETWORK_H
#define _NETWORK_H

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "30000"

#define MIN_TIME 1
#define MAX_TIME 30

#define MIN_WSIZE 1
#define MAX_WSIZE 7

#define MSG_LEN 128

typedef unsigned int seq_nr;
typedef enum {false, true} boolean;

typedef struct {
  seq_nr seq; 
  char info[MSG_LEN];
} frame;


// generate a UDP socket for server (receiver) 
// hostname as parameter
int get_udp_server_socket(char * );

// generate a UDP socket for client (sender)
// hostname and port number as parameters
int get_udp_client_socket(char * , char * );

// let a, b, and c be the parameters, respectively
// return true if b is in [a c)
boolean between(seq_nr, seq_nr, seq_nr);

#endif
