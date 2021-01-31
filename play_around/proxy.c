#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"


/* 
For question 2, the proxy uses TCP when communicating with the server and 
when communicating with the client. For question 3, the proxy uses TCP 
when communicating with the client and UDP when communicating with the server. 
*/
int main(int argc, char * argv[]) {
  int server_socket, client_socket;

  if (argc != 3) {
    printf("usage: ./proxy_server $(localhost) PORT");
    exit(1);
  }

  server_socket = tcp_server_socket_gen(PROXY_PORT);

  if (strcmp(argv[2], "30002") == 0) {
    client_socket = tcp_client(argv[1], argv[2]);
    // proxy_transmitting(client_socket, server_socket);
  } else {
    client_socket = udp_talker(argv[1], argv[2]);
  }
}