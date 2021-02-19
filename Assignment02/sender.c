#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"


void udp_sender(int socket) {
  struct sockaddr_storage addr;
  socklen_t addr_len;
  char buf[MSG_LEN];

  for (;;) {
    addr_len = sizeof(struct sockaddr_storage);
    printf("> ");
    scanf("%s", buf);

    sendto(socket, buf, MSG_LEN, 0, (struct sockaddr *) & addr, addr_len);
    bzero(buf, MSG_LEN);
    recvfrom(socket, buf, MSG_LEN, 0, (struct sockaddr * ) & addr, & addr_len);
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