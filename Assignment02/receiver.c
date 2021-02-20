// Jiaye Wang jiw561 11231145

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "network.h"



void udp_listener(int socket) {
  struct sockaddr_storage addr;
  socklen_t addr_len;
  char ack[MSG_LEN];
  char buf[MSG_LEN];
  char s[INET6_ADDRSTRLEN];

  int sequence_num;


  for (;;) {
    addr_len = sizeof(struct sockaddr_storage);
    // Received from sender
    FRAME frame;
    if(recvfrom(
          socket, &frame, sizeof frame, 0, 
          (struct sockaddr * ) & addr, & addr_len) == -1) {
      printf("receiver: failed to receive message\n");
    }

    strcpy(buf, frame.msg);
    sequence_num = frame.sequence_num;

    // Print the IP address of the sender. 
    printf("receiver: got packet from %s\n", 
        inet_ntop(
          addr.ss_family, 
          get_in_addr((struct sockaddr *) & addr), 
          s, sizeof s
        )
    );
    printf("sequence number: %d\nmessage: %s\n", sequence_num, buf);

    // Ask the receiver to input an ack. 
    printf("receiver: received message? (Y/N)\n");
    if (fgets(ack, sizeof ack, stdin) != NULL) {
      ack[strcspn(ack, "\n")] = 0;
    }

    if (ack[0] == 'Y') {
      printf("receiver: message successfully received\n");
    } else {
      printf("receiver: message not successfully received\n");
    }

    // Send back the ack to the receiver 
    sendto(socket, ack, MSG_LEN, 0, (struct sockaddr * ) & addr, addr_len);
  }
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("usage: ./receiver 30000 <window size>");
    exit(1);
  }

  int socket;
  int window_size;

  window_size = atoi(argv[2]);
  
  if (window_size < MIN_WSIZE || window_size > MAX_WSIZE) {
    printf(
        "error: window_size must between %d to %d, inclusive", 
        MIN_WSIZE, MAX_WSIZE);
    exit(1);
  }

  socket = get_udp_server_socket(PORT);

  printf("receiver is now listening...\n");
  udp_listener(socket);
  close(socket);
  return 0;
}
