// Jiaye Wang jiw561 11231145

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"

#include "cmd_processor.h"

void tcp_listener(int sockfd) {
  char buf[MSG_LEN];
  const char * response;

  for (;;) {
    bzero(buf, MSG_LEN);
    recv(sockfd, buf, sizeof buf, 0); // Receive message from the client

    response = get_weather(buf);
    printf("Received: %s\n", buf);
    printf("    Sent: %s\n", response);

    send(sockfd, response, MSG_LEN, 0); // Send back server response

    if (strncmp(buf, "quit", 4) == 0) { // Terminate if client send "quit"
      printf("TCP server exit...\n");
      break;
    }
  }
}

int main(int argc, char * argv[]) {
  int sockfd;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;

  if (argc != 1) {
    printf("usage: ./tcp_server\n");
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