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

int get_udp_server_socket() {
  int sockfd, rv;
  struct addrinfo hints;
  struct addrinfo * serverinfo, * p;

  memset( & hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = AI_PASSIVE; // For wildcard IP address

  rv = getaddrinfo(NULL, UDP_PORT, & hints, & serverinfo);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  // getaddrinfo() will return a list of address structures. 
  // Loop through all the result and make a socket. 
  for (p = serverinfo; p != NULL; p = p -> ai_next) {
    sockfd = socket(p -> ai_family, p -> ai_socktype, p -> ai_protocol);
    if (sockfd == -1) {
      perror("UDP server: socket");
      continue;
    }
    // Success binded. 
    if (bind(sockfd, p -> ai_addr, p -> ai_addrlen) == 0) {
      break;
    }
    close(sockfd);
  }
  // No address succeeded. 
  if (p == NULL) {
    fprintf(stderr, "UDP server: failed to bind socket\n");
    exit(EXIT_FAILURE);
  }
  // No longer need;
  freeaddrinfo(serverinfo);
  return sockfd;
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
  return 0;
}