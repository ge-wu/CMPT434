#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"

// Read datagram and send back corresponding weather information. 
void udp_listener(int socket) {
  int s;
  struct sockaddr_storage addr;
  socklen_t addr_len;
  ssize_t numbytes;
  char buf[MSG_LEN];

  for (;;) {
    addr_len = sizeof(struct sockaddr_storage);
    numbytes = recvfrom(
      socket, buf, MSG_LEN, 0, (struct sockaddr * ) & addr, & addr_len
    );
    // Ignore failed request
    if (numbytes == -1) {
      continue;
    }

    char host[NI_MAXHOST], service[NI_MAXSERV];
    s = getnameinfo(
      (struct sockaddr * ) & addr, addr_len,
      host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV
    );

    if (s == 0) {
      printf("Received %zd bytes from %s:%s\n", numbytes, host, service);
    }
    if (sendto(
        socket, buf, numbytes, 0, (struct sockaddr * ) & addr, addr_len
      ) != numbytes) {
      fprintf(stderr, "Error sending response\n");
    }
  }
}

int main(int argc, char * argv[]) {
  int socket;

  if (argc != 1) {
    printf("usage: ./udp_server");
    exit(1);
  }

  socket = get_udp_server_socket();
  printf("UDP server is now listening...\n");
  udp_listener(socket);
  close(socket);
  return 0;
}