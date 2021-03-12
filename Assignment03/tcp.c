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

int tcp_server_init(const char *host, const char *port) {
  int rv, sockfd;
  struct addrinfo hints;
  struct addrinfo *servinfo, *p;
  
  if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  for (p = servinfo; p != NULL; p=p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
      continue;
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
      continue;
    break;
  }
  if (p == NULL)
    return -2;
  freeaddrinfo(servinfo);
  return sockfd;
}

int tcp_client_init(const char *port) {
  int sockfd, rv, yes;
  struct addrinfo hints;
  struct addrinfo *servinfo, *p;

  yes = 1;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }

  for (p = servinfo; p != NULL; p=p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("Server: socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }
    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("Server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);
  if (p == NULL) {
    fprintf(stderr, "Server: failed to bind socket\n");
    exit(EXIT_FAILURE);
  }
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  return sockfd;
}

int tcp_accept(int sockfd) {
  socklen_t sin_size;
  struct sockaddr_storage their_addr;
  int new_fd;

  sin_size = sizeof(their_addr);
  new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size);
  if (new_fd == -1) {
    perror("accept");
    return -1;
  }
  return new_fd;
}
