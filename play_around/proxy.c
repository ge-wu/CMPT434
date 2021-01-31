#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

#include "cmd_processor.h"


void proxy_transmittor(int client_socket, int server_socket) {
  char buf[MSG_LEN];
  char temp[MSG_LEN];

  for (;;) {
    // Receive data from the client. 
    bzero(buf, sizeof buf);
    recv(server_socket, buf, MSG_LEN, 0);
    printf("From client: %s\n", buf);

    // [all] command only for the proxy server. It will send seven request to 
    // the UDP/TCP server, and concatenation seven responses. Finally, send back 
    // to the client. Without the proxy, [all] will be treated as invalid.
    if (strncmp(buf, "all", 3) == 0) {
      bzero(buf, sizeof buf);
      for (int i = 0; i < 7; i++) {
        write(client_socket, day[i], 3);
        bzero(temp, sizeof temp);
        read(client_socket, temp, MSG_LEN);
        // Formatting
        strcat(buf, day[i]);
        strcat(buf, ": ");
        strcat(buf, temp);
        // Add new line character if it is not last request response's message. 
        if (i != 6) {
          strcat(buf, "\n");
        }
      }
    } else {
      // Send the data from the client to the UDP/TCP server and 
      // get the server responses
      write(client_socket, buf, strlen(buf));
      bzero(buf, sizeof buf);
      read(client_socket, buf, MSG_LEN);
    }
    printf("From server: %s\n", buf);
    // Send back to the client
    send(server_socket, buf, MSG_LEN, 0);
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
  } else {
    printf("Error: incorrect port. Currently only support 30002 and 30003");
  }

  // Check if the proxy server is ready to listen and verification. 
  if (listen(server_socket, BACKLOG) == -1) {
    perror("TCP server: listen");
    exit(EXIT_FAILURE);
  }
  printf("Proxy server waiting for connection...\n");
  // Accept the connection (send the proxy server to the air)
  sin_size = sizeof their_addr;
  server_socket = accept(
    server_socket, (struct sockaddr * ) & their_addr, & sin_size
  );
  if (server_socket == -1) {
    perror("Proxy server: accept");
    exit(EXIT_FAILURE);
  }
  proxy_transmittor(client_socket, server_socket);

  close(server_socket);
  close(client_socket);
  return 0;
}