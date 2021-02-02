// Jiaye Wang jiw561 11231145

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"

void tcp_talker(int socket) {
  char buf[MSG_LEN];
  for (;;) {
    printf("> ");
    scanf("%s", buf);

    if (strlen(buf) + 1 > 5) {
      printf("Error: command too long\n");
      continue;
    }
    send(socket, buf, MSG_LEN, 0);
    bzero(buf, MSG_LEN);
    recv(socket, buf, MSG_LEN, 0);

    if (strncmp(buf, "exit", 4) == 0) {
      printf("TCP client exit...\n");
      break;
    }
    printf("%s\n", buf);
  }
}

int main(int argc, char * argv[]) {
  int socket;
  if (argc != 3) {
    printf("usage: ./tcp_client hostname port\n");
    exit(1);
  }

  socket = get_tcp_client_socket(argv[1], argv[2]);
  printf("--------------------------------------\n"
    "|        Weekly Weather Query        |\n"
    "|Usage:                              |\n"
    "|  $ <Mon|Tue|Wed|Thu|Fri|Sat|Sun>   |\n"
    "|<all> to check the weather list     |\n"
    "|  (only when connected to proxy)    |\n"
    "|Enter <quit> to quit                |\n"
    "--------------------------------------\n");

  tcp_talker(socket);
  close(socket);
  return 0;
}