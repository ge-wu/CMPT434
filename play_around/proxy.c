#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

void proxy_transmittor(int client_socket, int server_socket) {
  char buf[MSG_LEN];

  for (;;) {
    // Receive data from the client. 
    bzero(buf, sizeof buf);
    recv(server_socket, buf, MSG_LEN, 0);

    // Send to the server and get response from the server
    send(client_socket, buf, MSG_LEN, 0);
    bzero(buf, sizeof buf);
    recv(client_socket, buf, MSG_LEN, 0);

    // Send back to the client
    send(server_socket, buf, MSG_LEN, 0);
  }
}

void udp_talker(int socket) {
  char buf[MSG_LEN];
  size_t len;
  ssize_t numberbyte;
  for (;;) {
    printf("> ");
    scanf("%s", buf);
    len = strlen(buf) + 1;
    if (len + 1 > MSG_LEN) {
      printf("Error: message is too long\n");
      continue;
    }
    if (write(socket, buf, len) != len) {
      fprintf(stderr, "partial/failed write\n");
      exit(EXIT_FAILURE);
    }
    numberbyte = read(socket, buf, MSG_LEN);
    if (numberbyte == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    printf("Received %zd bytes: %s\n", numberbyte, buf);
  }
}

int main(int argc, char * argv[]) {
  int server_socket, client_socket;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;

  if (argc != 3) {
    printf("usage: ./proxy_server $(localhost) PORT");
    exit(EXIT_FAILURE);
  }

  // For question 2, the proxy uses TCP when communicating with the server and 
  // when communicating with the client. For question 3, the proxy uses TCP 
  // when communicating with the client and UDP when communicating with the 
  // server. Hence, the proxy's server will always be TCP server. 
  server_socket = get_tcp_server_socket(PROXY_PORT);

  if (strncmp(argv[2], "30002", 5) == 0) {  // Connect to the TCP server
    client_socket = get_tcp_client_socket(argv[1], argv[2]);
  } else if (strncmp(argv[2], "30003", 5) == 0) {  // Connect to the UDP server. 
    client_socket = get_udp_client_socket(argv[1], argv[2]);
    udp_talker(client_socket);
  } else {
    printf("Error: incorrect port. Currently only support 30002 and 30003");
  }

  // // Check if the proxy server is ready to listen and verification. 
  // if (listen(server_socket, BACKLOG) == -1) {
  //   perror("TCP server: listen");
  //   exit(EXIT_FAILURE);
  // }
  // printf("Proxy server waiting for connection...\n");
  // // Accept the connection
  // sin_size = sizeof their_addr;
  // server_socket = accept(
  //   server_socket, (struct sockaddr * ) & their_addr, & sin_size
  // );
  // if (server_socket == -1) {
  //   perror("Proxy server: accept");
  //   exit(EXIT_FAILURE);
  // }
  // proxy_transmittor(client_socket, server_socket);

  close(server_socket);
  close(client_socket);
  return 0;
}