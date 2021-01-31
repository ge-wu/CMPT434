#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"

void tcp_listener(int sockfd) {
  char buf[MSG_LEN];

  for (;;) {
    bzero(buf, MSG_LEN);
    recv(sockfd, buf, sizeof buf, 0);
    printf("Received: %s\n \tSent: ", buf);
    send(sockfd, buf, MSG_LEN, 0);
  }
}

int main(int argc, char * argv[]) {
  int sockfd;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;

  if (argc != 1) {
    printf("usage: ./tcp_server");
    exit(1);
  }

  sockfd = get_tcp_server_socket(TCP_PORT);

  // Check if the server is ready to listen and verification. 
  if (listen(sockfd, BACKLOG) == -1) {
    perror("TCP server: listen");
    exit(EXIT_FAILURE);
  }
  printf("TCP server waiting for connection...\n");
  // Accept the connection
  sin_size = sizeof their_addr;
  sockfd = accept(sockfd, (struct sockaddr * ) & their_addr, & sin_size);
  if (sockfd == -1) {
    perror("TCP server: accept");
    exit(EXIT_FAILURE);
  }

  tcp_listener(sockfd);
  close(sockfd);
  return 0;
}