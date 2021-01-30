#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"

void tcp_talker(int socket) {
  char buf[MSG_LEN];
  for (;;) {
    printf("> ");
    scanf("%s", buf);

    if (strlen(buf) + 1 > MSG_LEN) {
      printf("Error: message is too long");
      continue;
    }
    send(socket, buf, MSG_LEN, 0);
    bzero(buf, MSG_LEN);
    recv(socket, buf, MSG_LEN, 0);
    printf("%s\n", buf);
  }
}

int main(int argc, char * argv[]) {
  int socket;
  if (argc != 3) {
    printf("usage: ./tcp_client $(localhost) PORT\n");
    exit(1);
  }

  socket = get_tcp_client_socket(argv[1], argv[2]);
  printf("--------------------------------------\n"
    "|        Weekly Weather Query        |\n"
    "|Usage:                              |\n"
    "|  $ <Mon|Tue|Wed|Thu|Fri|Sat|Sun>   |\n"
    "|<all> to check the weather list     |\n"
    "|Enter <exit> to quit                |\n"
    "--------------------------------------\n");

  tcp_talker(socket);
  close(socket);
  return 0;
}