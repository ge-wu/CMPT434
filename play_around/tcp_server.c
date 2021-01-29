#include <stdio.h>
#include <stdlib.h>

#include "network.h"

int main(int argc, char *argv[]) {
	int sockfd; 
	if (argc != 1) {
		printf("usage: ./tcp_server");
		exit(1);
	}
	sockfd = tcp_server_socket_gen(TCP_PORT);
	response_tcp_server(sockfd);
	return 0;
}