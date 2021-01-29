#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

char *weather_lst[7] = {
    "Blizzard     ", 
    "Comet Strike ", 
    "Drought      ", 
    "Comet Strike ", 
    "Drought      ", 
    "Flood        ", 
    "Meteor Shower"
};

char *day[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};


char *get_weather(char msg[]) {
  size_t n = strlen(msg);
  if (*msg && msg[n - 1] == '\n') {
    msg[n - 1] = '\0';
  }
  for (int i = 0; i < 7; i++) {
    if (strcmp(day[i], msg) == 0) {
      return weather_lst[i];
    }
  }
  return "Error: invalid input.";
}

void send_tcp_message(int sockfd) {
  char buf[MSG_LEN];
  while (1) {
    printf("$ ");
    scanf("%s", buf);
    send(sockfd, buf, strlen(buf), 0);

    bzero(buf, sizeof buf);
    recv(sockfd, buf, sizeof buf, 0);
    if ((strncmp(buf, "disconnect", 10)) == 0) {
      printf("TCP client exit\n");
      return;
    }
    printf("%s\n", buf);
  }
}

void response_tcp_server(int sockfd) {
  char buff[MSG_LEN]; 
  char *response;

  while (1) { 
    bzero(buff, sizeof buff);
    recv(sockfd, buff, sizeof buff, 0); 
    printf("From client: %s\n", buff); 

    if (strncmp("exit", buff, 4) == 0) {
      send(sockfd, "disconnect", 10 + 1, 0);
      printf("Server exit\n");
      break;
    } 
    response = get_weather(buff);
    send(sockfd, response, strlen(response), 0); 
  } 
}

void send_udp_message(int sockfd, struct addrinfo *p) {
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

    if ((strncmp(buf, "disconnect", 10)) == 0) {
      printf("UDP talker exit\n");
      break;
    }
    printf("%s\n", buf);
  }
}

void response_udp_message(int sockfd) {
  struct sockaddr_storage their_addr;
  socklen_t addr_len;
  char buf[MSG_LEN];
  char *response;
  int numbytes;

  addr_len = sizeof their_addr;
  while (1) {
    if ((numbytes = recvfrom(sockfd, buf, MSG_LEN - 1, 0, 
      (struct sockaddr *)& their_addr, &addr_len)) == -1) {
        perror("UDP listener: recvfrom");
        return;
    }
    if (strncmp("exit", buf, 4) == 0) {
      sendto(sockfd, "disconnect", 10, 0, (struct sockaddr *)& their_addr, addr_len);
      printf("UDP listener exit\n");
      break;
    }
    response = get_weather(buf);
    
    if (sendto(sockfd, response, strlen(response), 0, 
      (struct sockaddr *)& their_addr, addr_len) == -1) {
        perror("UDP listener: sendto");
        exit(1);
    }
  }
}

/* UDP server */
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
  response_udp_message(sockfd);
  close(sockfd);

  return 0;
}

/* UDP client */
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

  send_udp_message(sockfd, p);

  freeaddrinfo(serverinfo);
  close(sockfd);

  return 0;
}

/* TCP client */
int tcp_client(char* hostname, char* port) {
  int sockfd, rv;
  struct addrinfo hints, *serverinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(hostname, port, &hints, &serverinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}	
  // loop through all the results and connect to the first we can
	for(p = serverinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}
    break;
	}

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }  
  freeaddrinfo(serverinfo);

  printf("--------------------------------------\n"
      "|        Weekly Weather Query        |\n"
      "|Usage:                              |\n"
      "|  $ <Mon|Tue|Wed|Thu|Fri|Sat|Sun>   |\n"
      "|<all> to check the weather list     |\n"
      "|Enter <exit> to quit                |\n"
      "--------------------------------------\n");
  send_tcp_message(sockfd);
  close(sockfd);

  return 0;
}

int tcp_server() {
	int sockfd, new_fd; 
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; 
	socklen_t sin_size;
	int yes=1;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, TCP_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
    printf("server: accept failed\n");
		exit(0);
	} else {
		printf("server: accepted the connection.\n");
	}

  response_tcp_server(new_fd);
	close(new_fd);
	return 0;
}

