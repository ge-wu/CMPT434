// Jiaye Wang jiw561 11231145

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
 
typedef struct {char id; char data[10];} packet;

packet buffer[MAX_ID];

int count_space_used() {
  int ans = 0;
  for (int i = 0; i < MAX_ID; i++) {
    if (buffer[i].id != '#') 
      ans++;
  }
  return ans;
}

int main(int argc , char *argv[]) {
    if (argc != 5) {
        printf("usage: ./sensor <ID> <port> <storage space> <msg>\n");
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) != 1 || (argv[1][0] < 'A' || argv[1][0] > 'Z')) {
      printf("error: ID must be a uppercase English letter\n");
      exit(EXIT_FAILURE);
    }

    if (atoi(argv[2]) < 30000 || atoi(argv[2]) > 40000) {
      printf("error: port must between 30000 to 40000, inclusive\n");
      exit(EXIT_FAILURE);
    }

    if (atoi(argv[3]) <= 0) {
      printf("error: storage space must be at least 1\n");
      exit(EXIT_FAILURE);
    }
    
    if (strlen(argv[4]) > 10) {
      printf("error: msg cannot exceed 10 characters \n");
      exit(EXIT_FAILURE);
    }

    // dummy variables for TCP
    socklen_t sin_size;
    struct sockaddr_in their_addr;

    int sockfd, new_sockfd, client_sockfd;
    int max_sd;
    int storage_space = atoi(argv[3]);
    char id = argv[1][0];

    sockfd = tcp_server_init(argv[2]);

    fd_set readfds;

    // initialize sensor buffer, 
    for (int i = 0; i < MAX_ID; i++) {
        buffer[i].id = '#';
        strcpy(buffer[i].data, "#");
    }
    buffer[id - 'A'].id = id;
    strcpy(buffer[id - 'A'].data, argv[4]);

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Sensor %c is up to the air...\n", id);

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
          if (new_sockfd == -1) {
              perror("accept");
              exit(EXIT_FAILURE);
          }
          printf("Incoming connection...\n");

          for (int i = 0; i < 26; i++) {
            packet p;
            if (recv(new_sockfd, &p, sizeof(packet), 0) < 0) 
              perror("server recv");
            if (p.id != '#') {
              // Add to the buffer if we still have free space in the storage
              printf("Receive from %c: %s\n", p.id, p.data);
              if (count_space_used() < storage_space + 1) {
                buffer[p.id - 'A'].id = p.id;
                strcpy(buffer[p.id - 'A'].data, p.data);
                printf("Buffered successfully\n");
              } else {
                printf("Storage is full\n");
              }
            }
          }
          close(new_sockfd);
        }

        // handle user input
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            char input[64], port[10], cmd;
            fgets(input, sizeof(input), stdin);  // get user input
            sscanf(input, "%c%s", &cmd, port);

            if (cmd == 'Q') break;  // terminate the sensor
            // start to transmit data between sensors. 
            if (cmd == 'C') {
              // establish the connection
              if ((client_sockfd = tcp_client_init("tux8", port)) != -1) {
                for (int i = 0; i < 26; i++) {
                  if (send(client_sockfd, &buffer[i], sizeof(packet), 0) < 0)
                      perror("send");
                  if (buffer[i].id != '#') 
                    printf("Send from %c: %s\n", buffer[i].id, buffer[i].data);
                }
                close(client_sockfd);
              }
            } 
            // transmit data to the base station and buffer will be reset.
            else if (cmd == 'B') {
              printf("Start to transmit data to base station...\n");
              for (int i = 0; i < 26; i++) {
                  if (buffer[i].id == '#') continue;
                  printf("ID: %c: %s\n", buffer[i].id, buffer[i].data);
                  // Do not clean sensor's own data
                  if (buffer[i].id != id) {
                    buffer[i].id = '#';
                    bzero(buffer[i].data, 11);
                  }
              }
              printf("Transmission complete...\n");
            } else {
              printf("Unrecognized command\n");
            }
        }
        printf("-------------------------------------------\n");  // divide line
    }

    close(sockfd);
    printf("Bye :)\n");
    return 0;
}

