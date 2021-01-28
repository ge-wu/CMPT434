#include <stdio.h>
#include <stdlib.h>

#include "network.h"

void udp_message_sender(int sockfd, struct addrinfo *p) {
  char buf[MSG_LEN];
  int numbytes;

  while (1) {
    printf("$ ");
    scanf("%s", buf);
    if (sendto(sockfd, buf, MSG_LEN - 1, 0, p->ai_addr, p->ai_addrlen) == -1) {
      perror("UPD talker: sendto");
      exit(1);
    }
    bzero(buf, sizeof buf);
    numbytes = recvfrom(sockfd, buf, MSG_LEN - 1, 0, p->ai_addr, &p->ai_addrlen);
    buf[numbytes] = '\0';
  }
}

void udp_message_responser(int sockfd) {
  struct sockaddr_storage their_addr;
  socklen_t addr_len;
  char buf[MSG_LEN];
  int numbytes;

  addr_len = sizeof their_addr;
  while (1) {
    if ((numbytes = recvfrom(sockfd, buf, MSG_LEN - 1, 0, 
      (struct sockaddr *)& their_addr, &addr_len)) == -1) {
        perror("UDP listener: recvfrom");
        return;
    }
    if (sendto(sockfd, "hello", 5, 0, 
      (struct sockaddr *)& their_addr, addr_len) == -1) {
        perror("UDP listener: sendto");
        exit(1);
    }
    printf("Hello message sent.\n");
  }
}

int udp_listener() {
  int sockfd, rv;
  struct addrinfo hints, *serverinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, UDP_PORT, &hints, &serverinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
  }
	for(p = serverinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}
		break;
	}
  if (p == NULL) {
    fprintf(stderr, "UDP listener: failed to bind socket\n");
    return 2;
  }
  freeaddrinfo(serverinfo);

  printf("UDP listener: wainting to recvfrom...\n");

  udp_message_responser(sockfd);
  close(sockfd);

  return 0;
}

int udp_talker(char* hostname, char* port) {
  int sockfd, rv;
  struct addrinfo hints, *serverinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(hostname, port, &hints, &serverinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

  for (p = serverinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("UDP talker: socket");
      continue;
    }
    break;
  }
  if (!p) {
    fprintf(stderr, "UDP talker: failed to create socket\n");
    return 2;
  }

  udp_message_sender(sockfd, p);

  freeaddrinfo(serverinfo);
  close(sockfd);

  return 0;
}