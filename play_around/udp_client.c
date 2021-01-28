#include <stdio.h>
#include <stdlib.h>

#include "network.h"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr,"usage: ./udp_client $(hostname) 30003\n");
		exit(1);
	}

  udp_talker(argv[1], argv[2]);
  return 0;
}