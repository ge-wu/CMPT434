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
    bzero(buf, sizeof buf);
    read(socket, buf, MSG_LEN);
    printf("From server: %s\n", buf);
  }
}

int main(int argc, char * argv[]) {
  if (argc != 5) {
    printf("usage: ./sender <IP address> 30000 <window size> <timeout>\n");
    printf("    please check README for more usage detail :)");
    exit(1);
  }

  int socket;
  int window_size, timeout;

  window_size = atoi(argv[3]);
  timeout = atoi(argv[4]);

  if (window_size < MIN_WSIZE || window_size > MAX_WSIZE) {
    printf(
        "error: window size must between %d to %d\n, inclusive", 
        MIN_WSIZE, MAX_WSIZE
        );
    exit(1);
  }

  if (timeout < MIN_TIME || timeout > MAX_TIME) {
    printf(
        "error: timeout must between %d to %d, inclusive", 
        MIN_TIME, MAX_TIME
        );
    exit(1);
  }


  socket = get_udp_client_socket(argv[1], argv[2]);
  udp_sender(socket);
  close(socket);
}
