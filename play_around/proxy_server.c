#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"


void proxy_transmitting(int client_sockfd, int server_sockfd) {
  char msg[MSG_LEN] = "";
  char *day[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

  while (1) {
    bzero(msg, sizeof msg);
    recv(server_sockfd, msg, sizeof msg, 0);

    if ((strncmp(msg, "exit", 4)) == 0) {
      send(client_sockfd, "exit", 4, 0);
      send(server_sockfd, "disconnect", 10 + 1, 0);
      printf("Proxy exit\n");
      break;
    }
    if ((strncmp(msg, "all", 3)) == 0) {
      bzero(msg, sizeof msg);
      char temp[MSG_LEN] = "";
      for (int i = 0; i < 7; i++) {
        send(client_sockfd, day[i], 3, 0);
        bzero(temp, sizeof temp);
        recv(client_sockfd, temp, sizeof temp, 0);
        // Formatting
        strcat(msg, day[i]);
        strcat(msg, ": ");
        strcat(msg, temp);
        if (i != 6)
          strcat(msg, "\n");
      }
    } else {
      send(client_sockfd, msg, strlen(msg), 0);
      bzero(msg, sizeof msg);
      recv(client_sockfd, msg, sizeof msg, 0);
    }
    send(server_sockfd, msg, strlen(msg), 0);
  }
}

int main(int argc, char * argv[]) {
  int server_socket, client_socket;

  if (argc != 3) {
    printf("usage: ./proxy_server $(localhost) PORT");
    exit(1);
  }

  if (strcmp(argv[2], "30002") == 0) {
    client_socket = tcp_client(argv[1], argv[2]);
    server_socket = tcp_server_socket_gen(PROXY_PORT);
    proxy_transmitting(client_socket, server_socket);
  } else {

  }
}