#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <poll.h>

#include "tcp.h"

#define MSG_LEN 64
#define MAX_ID 26
 
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct {char id; char data[11];} packet;

packet buffer[MAX_ID];

void transit_to_basestation() {
  printf("Start to transmit data to base station...\n");
  for (int i = 0; i < 26; i++) {
      if (buffer[i].id == '#') continue;
      printf("ID: %d\nData: %s\n", buffer[i].id, buffer[i].data);
      buffer[i].id = '#';
      bzero(buffer[i].data, 11);
  }
  printf("Transmission complete...\n");
}

int main(int argc , char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: ./sensor <ID> <port> <storage space> <msg>\n");
        exit(EXIT_FAILURE);
    }

    // dummy variables for TCP
    socklen_t sin_size;
    struct sockaddr_in their_addr;

    int sockfd, new_sockfd, client_sockfd;
    int max_sd;
    char id = argv[1][0];

    sockfd = tcp_server_init(argv[2]);

    fd_set readfds;

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Sensor %s is up to the air...\n", argv[1]);

    // initialize sensor buffer, 
    // where "#" ID and empty data represents empty entry of the buffer
    for (int i = 0; i < MAX_ID; i++) {
        buffer[i].id = '#';
        strcpy(buffer[i].data, "#");
    }
    buffer[id - 'A'].id = id;
    strcpy(buffer[id - 'A'].data, argv[4]);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);
        max_sd = sockfd;

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
          perror("select");
          exit(EXIT_FAILURE);
        }

        // incoming connection
        if (FD_ISSET(sockfd, &readfds)) {
          new_sockfd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
          printf("Incoming connection...\n");
          if (new_sockfd == -1) {
              perror("accept");
              exit(EXIT_FAILURE);
          }
          for (int i = 0; i < 26; i++) {
            packet p;
            if (recv(new_sockfd, &p, sizeof(packet), 0) < 0) 
              perror("recv");
            if (p.id != '#') 
              printf("Receive from %c: %s\n", p.id, p.data);
          }
          /* close(new_sockfd); */
        }
        // handle user input
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            char input[64];
            fgets(input, sizeof(input), stdin);
            printf("Your input: %s", input);
            // terminate the sensor.
            if (input[0] == 'Q') break;

            // start to transmit data between sensors. 
            if (input[0] == 'C') {
              // establish the connection
                client_sockfd = tcp_client_init("tux8", "30000");
                for (int i = 0; i < 26; i++) {
                  if (send(client_sockfd, &buffer[i], sizeof(packet), 0) < 0) {
                      perror("send");
                  }
                }
            } 
            // transmit data to the base station and buffer will be reset.
            else if (input[0] == 'B') {
              transit_to_basestation();
            } else {
                printf("Unrecognized command\n");
            }
        }
    }

    close(sockfd);
    printf("Bye :)\n");
    return 0;
}

