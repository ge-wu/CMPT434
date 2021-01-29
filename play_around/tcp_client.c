#include <stdio.h>
#include <stdlib.h>

#include "network.h"


int main(int argc, char *argv[]) {
	tcp_client(argv[1], argv[2]);
	return 0;
}
