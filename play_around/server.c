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

#define PORT "30001"
#define BACKLOG 10
#define LEN_MAX 1024

char *weather_lst[7] = {
"Monday's Weather:    Blizzard        Intensity: I", 
"Tuesday's Weather:   Comet Strike    Intensity: II", 
"Wednesday's Weather: Drought         Intensity: III", 
"Thursday's Weather:  Comet Strike    Intensity: IV", 
"Friday's Weather:    Drought         Intensity: V", 
"Saturday's Weather:  Flood           Intensity: VI", 
"Sunday's Weather:    Meteor Shower   Intensity: VII", 
};

char *date[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

char *server_weather_response(char msg[]) {
  size_t n = strlen(msg);
  if (*msg && msg[n - 1] == '\n') 
    msg[n - 1] = '\0';

  for (int i = 0; i < 7; i++) {
    if (strcmp(date[i], msg) == 0)
      return weather_lst[i];
  }
  return "Error: invalid input";
}

void listener(int sockfd) {
  char buff[LEN_MAX]; 
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
    response = server_weather_response(buff);
    send(sockfd, response, strlen(response), 0); 
  } 
}

void sigchld_handler(int s) {
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

int main(void) {
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	// char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
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

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    printf("server: sigaction failed\n");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

	if (new_fd == -1) {
    printf("server: accept failed\n");
		exit(0);
	} else {
		printf("server: accepted the client.\n");
	}

  listener(new_fd);
	close(new_fd);
	return 0;
}
