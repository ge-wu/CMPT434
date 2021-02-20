// Jiaye Wang jiw561 11231145

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"

int get_udp_server_socket(char * port) {
  int sockfd, rv;
  struct addrinfo hints;
  struct addrinfo * serverinfo, * p;

  memset( & hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = AI_PASSIVE; // For wildcard IP address

  rv = getaddrinfo(NULL, port, & hints, & serverinfo);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  // getaddrinfo() will return a list of address structures. 
  // Loop through all the result and make a socket. 
  for (p = serverinfo; p != NULL; p = p -> ai_next) {
    sockfd = socket(p -> ai_family, p -> ai_socktype, p -> ai_protocol);
    if (sockfd == -1) {
      perror("UDP server: socket");
      continue;
    }
    // Success binded. 
    if (bind(sockfd, p -> ai_addr, p -> ai_addrlen) == 0) {
      break;
    }
    close(sockfd);
  }
  // No address succeeded. 
  if (p == NULL) {
    fprintf(stderr, "UDP server: failed to bind socket\n");
    exit(EXIT_FAILURE);
  }
  // No longer need;
  freeaddrinfo(serverinfo);
  return sockfd;
}

int get_udp_client_socket(char * hostname, char * port) {
  struct addrinfo hints;
  struct addrinfo * serverinfo, * p;
  int sockfd, rv;

  // Obtain address(es) matching host/port
  memset( & hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6;
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket;
  hints.ai_flags = 0;

  rv = getaddrinfo(hostname, port, & hints, & serverinfo);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }
  // getaddrinfo() will return a list of address structures. 
  // Loop through all the result and make a socket. 
  for (p = serverinfo; p != NULL; p = p -> ai_next) {
    sockfd = socket(p -> ai_family, p -> ai_socktype, p -> ai_protocol);
    if (sockfd == -1) {
      continue;
    }
    // Success
    if (connect(sockfd, p -> ai_addr, p -> ai_addrlen) != -1) {
      break;
    }
    close(sockfd);
  }
  if (p == NULL) {
    fprintf(stderr, "UDP client: failed to create socket\n");
    exit(EXIT_FAILURE);
  }
  // No longer need
  freeaddrinfo(serverinfo);
  return sockfd;
}

void * get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
