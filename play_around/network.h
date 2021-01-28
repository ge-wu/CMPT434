#ifndef _NETWORK_H
#define _NETWORK_H

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MSG_LEN 1024
#define TCP_PORT "30002"
#define UDP_PORT "30003"

// char *weather_lst[7] = {
//     "Blizzard     ", 
//     "Comet Strike ", 
//     "Drought      ", 
//     "Comet Strike ", 
//     "Drought      ", 
//     "Flood        ", 
//     "Meteor Shower"
// };

// char *date[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

int udp_talker(char*, char*);
int udp_listener();
void udp_message_sender(int, struct addrinfo *);
void udp_message_responser(int);

// void tcp_server();
// void tcp_listener();

#endif

