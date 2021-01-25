#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

// #include "network.h"

#define PORT "30001"
#define BACKLOG 10
#define LEN_MAX 1024

void proxy_transmitting(int client_sockfd, int server_sockfd) {
  char msg[LEN_MAX];

  while (1) { 
    // receive message from client
    bzero(msg, sizeof msg);
    recv(server_sockfd, msg, sizeof msg, 0);
    if ((strncmp(msg, "exit", 4)) == 0) {
      send(client_sockfd, "exit", 4, 0);
      send(server_sockfd, "disconnect", 10 + 1, 0);
      printf("Proxy exit\n");
      break;
    }
    // send to the server and get the response form server
    send(client_sockfd, msg, strlen(msg), 0);
    bzero(msg, sizeof msg);
    recv(client_sockfd, msg, sizeof msg, 0);

    // send back to the client
    send(server_sockfd, msg, strlen(msg), 0);
  } 
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s) {
	(void)s;
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

int main(int argc, char *argv[]) {
	int server_sockfd, client_sockfd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	int rv;
  
  char s[INET6_ADDRSTRLEN];

  struct addrinfo client_hints, *client_serverinfo, *client_p;
  int client_rv;

  if (argc != 3) {
    fprintf(stderr, "usage: ./proxy <hostname> <port>\n");
    exit(1);
  }

  memset(&client_hints, 0, sizeof client_hints);
  client_hints.ai_family = AF_UNSPEC;
  client_hints.ai_socktype = SOCK_STREAM;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

  if ((client_rv = getaddrinfo(argv[1], argv[2], &client_hints, &client_serverinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (client_p = client_serverinfo; client_p != NULL; client_p = client_p->ai_next) {
    if ((client_sockfd = socket(client_p->ai_family, client_p->ai_socktype, 
      client_p->ai_protocol)) == -1) {
        perror("proxy: socket");
        continue;
      }
      if (connect(client_sockfd, client_p->ai_addr, client_p->ai_addrlen) == -1) {
        perror("proxy: connect");
        close(client_sockfd);
        continue;
      }
      break;
  }
  if (client_p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }
  inet_ntop(client_p->ai_family, get_in_addr((struct sockaddr *)client_p->ai_addr),
    s, sizeof s);
  printf("proxy: connecting to %s\n", s);
  freeaddrinfo(client_serverinfo);


	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((server_sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(server_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "proxy: failed to bind\n");
		exit(1);
	}

	if (listen(server_sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    printf("proxy: sigaction failed\n");
		exit(1);
	}

	printf("proxy: waiting for connections...\n");

	sin_size = sizeof their_addr;
	server_sockfd = accept(server_sockfd, (struct sockaddr *)&their_addr, &sin_size);

	if (server_sockfd == -1) {
    printf("proxy: accept failed\n");
		exit(0);
	} else {
		printf("proxy: accepted the client.\n");
	}

  proxy_transmitting(client_sockfd, server_sockfd);

	close(server_sockfd);
  close(client_sockfd);
	return 0;
}
