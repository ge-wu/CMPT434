// Jiaye Wang jiw561 11231145
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "tcp.h"

#define MSG_LEN 64

typedef struct {
  char id;
  char *data;
} packet;

packet buffer[26];


int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("usage: ./sensor <ID> <port> <storage space> <data>\n");
    exit(EXIT_FAILURE);
  }

  char id;
  char *port, *cur_port;
  char input[MSG_LEN];
  char cur_cmd;

  int server_sockfd, client_sockfd, new_sockfd;
  int storage_space;
  int space_cnt;  // count how many space are being used.

  packet p;

  id = argv[1][0];
  if (strlen(argv[1]) != 1 || (id < 'A' || id > 'Z')) {
    printf("error: ID should be a single upper case letter\n");
    exit(EXIT_FAILURE);
  }

  port = argv[2];
  if (atoi(port) > 40000 || atoi(port) < 30000) {
    printf("error: port should between 30000 ~ 40000, inclusive\n");
    exit(EXIT_FAILURE);
  }
  storage_space = atoi(argv[3]);

  // Initialize the buffer. Note there are max of 26 IDs, and we can use 
  // (ID - 'A') to get the index. E.g.: 'A' - 'A' = 0, 'B' - 'A' = 1, etc.
  for (int i = 0; i < 26; i++) {
    buffer[i].id = '#';
    buffer[i].data = "";
  }
  buffer[id - 'A'].id = id;
  buffer[id - 'A'].data = argv[5];

  server_sockfd = tcp_server_init("potato", port);

  for (;;) {
    cur_cmd = '#';
    cur_port = "";
    fgets(input, sizeof(input), stdin);  // read user input
    sscanf(input, "%c %s", &cur_cmd, cur_port);  // parse user input
    
    // terminate the process
    if (cur_cmd == 'Q') {
      printf("Bye~ :)\n");
      break;
    }

    if (cur_cmd == 'C') {
      client_sockfd = tcp_client_init(cur_port);
      new_sockfd = tcp_accept(client_sockfd);
      for (int i = 0; i < 26; i++) {
        if (buffer[i].id == '#') continue;  // skip empty buffer
        send(new_sockfd, &buffer[i], sizeof(packet), 0);
        printf("ID: %c\nData:%s\n", buffer[i].id, buffer[i].data);
        recv(new_sockfd, &p, sizeof(packet), 0);
        printf("ID: %c\nData:%s\n", p.id, p.data);
        printf("-------------------------");
      }
    } else if (cur_cmd == 'B') {
      // print each data packet that it has buffered. Such data packets 
      // are now considered to have been delivered to the base station. 
      // That is, the packet will be removed from the buffer. 
      for (int i = 0; i < storage_space; i++) {
        if (buffer[i].id == '#') continue;  // skip empty buffer
        printf("ID: %c\nData:%s\n", buffer[i].id, buffer[i].data);
      }
      space_cnt = 0;
    } else {
      printf("error: unrecognized input\n");
    }

  }

  return 0;

}
