#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"


void udp_sender(int socket) {
  char buf[MSG_LEN];

  for (;;) {
    printf("> ");
    scanf("%s", buf);

    write(socket, buf, sizeof buf);
    bzero(buf, MSG_LEN);
    read(socket, buf, sizeof buf);
  }
}

int main(int argc, char * argv[]) {
  int socket;
  if (argc != 5) {
    printf("usage: ./sender <IP address> 30000 <window size> <time out>\n");
    printf("    please check README for more usage detail :)");
    exit(1);
  }

  socket = get_udp_client_socket(argv[1], argv[2]);
  udp_sender(socket);
  close(socket);
}
