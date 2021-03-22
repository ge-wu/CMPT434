// Jiaye Wang jiw561 11231145

#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcp.h"

int tcp_server_init(const char *port) {
  int sockfd, rv;
  int yes = 1;
  struct addrinfo hints, * serverinfo, * p;

  memset( & hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // Stream socket
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
      perror("socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }
    if (bind(sockfd, p -> ai_addr, p -> ai_addrlen) == -1) {
      close(sockfd);
      perror("bind");
      continue;
    }
    // Success binded.
    break;
  }
  // No longer need
  freeaddrinfo(serverinfo);

  // No address succeeded.
  if (p == NULL) {
    fprintf(stderr, "failed to bind socket\n");
    exit(EXIT_FAILURE);
  }
  return sockfd;
}

int tcp_client_init(const char *hostname, const char *port) {
  struct addrinfo hints;
  struct addrinfo * serverinfo, * p;
  int sockfd, rv;

  memset( & hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // Stream socket;

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
      perror("socket");
      continue;
    }
    if (connect(sockfd, p -> ai_addr, p -> ai_addrlen) == -1) {
      close(sockfd);
      perror("connect");
      continue;
    }
    break;
  }

  if (p == NULL) {
    return -1;
  }
  // No longer need
  freeaddrinfo(serverinfo);
  return sockfd;
}


