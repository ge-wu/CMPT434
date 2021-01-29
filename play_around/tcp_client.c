#include <stdio.h>
#include <stdlib.h>

#include "network.h"


int main(int argc, char *argv[]) {
	int socket;
	if (argc != 3) {
		printf("usage: ./tcp_client $(localhost) 30001");
		exit(1);
	}
	printf("--------------------------------------\n"
		"|        Weekly Weather Query        |\n"
		"|Usage:                              |\n"
		"|  $ <Mon|Tue|Wed|Thu|Fri|Sat|Sun>   |\n"
		"|<all> to check the weather list     |\n"
		"|Enter <exit> to quit                |\n"
		"--------------------------------------\n");
	socket = tcp_client(argv[1], argv[2]);
	send_tcp_message(socket);
	return 0;
}
