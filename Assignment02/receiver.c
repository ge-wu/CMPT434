// Jiaye Wang jiw561 11231145

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"


void udp_listener(int socket) {
  struct sockaddr_storage addr;
  socklen_t addr_len;
  char buf[MSG_LEN];
  const char * response = "Hello";

  for (;;) {
    addr_len = sizeof(struct sockaddr_storage);
    recvfrom(socket, buf, MSG_LEN, 0, (struct sockaddr * ) & addr, & addr_len);

    printf("Received: %s\n", buf);
    printf("    Sent: %s\n", response);

    sendto(socket, response, MSG_LEN, 0, (struct sockaddr * ) & addr, addr_len);
    if (strncmp(buf, "quit", 4) == 0) {
      printf("UDP server exit...\n");
      break;
    }
  }
}

int main(int argc, char * argv[]) {
  int socket;

  if (argc != 3) {
    printf("usage: ./receiver 30000 <window size>");
    exit(1);
  }

  socket = get_udp_server_socket(PORT);
  printf("UDP server is now listening...\n");
  udp_listener(socket);
  close(socket);
  return 0;
}
