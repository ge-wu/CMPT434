// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"



void receiver(int socket, int window_size) {
  struct sockaddr_storage addr;
  socklen_t addr_len;
  char ack[MSG_LEN];
  char buf[MSG_LEN];
  char s[INET6_ADDRSTRLEN];

  int sequence_num;
  QUEUE *buffer;

  buffer = create_queue(sizeof(FRAME));
  sequence_num = 0;

  for (;;) {
    addr_len = sizeof(struct sockaddr_storage);
    FRAME frame;
    recvfrom(socket, &frame, sizeof frame, 
        0, (struct sockaddr * ) & addr, & addr_len);
    // Parse the message and sequence number from the frame. 
    strcpy(buf, frame.msg);
    sequence_num = frame.sequence_num;

    printf("sequence number: %d\nmessage: %s\n", sequence_num, buf);
    push(buffer, &frame);

    // Ask the receiver to input an ack. 
    printf("(R) received message? (Y/N): ");
    if (fgets(ack, sizeof ack, stdin) != NULL) {
      ack[strcspn(ack, "\n")] = 0;
    }

    if (ack[0] == 'Y') {
      sendto(socket, &sequence_num, sizeof(int), 
          0, (struct sockaddr * ) & addr, addr_len);
      printf("(R) acknowledge for frame %d sent\n", sequence_num);
    } else {
      printf("receiver: message not successfully received\n");
    }
    // Send back the ack to the receiver 
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
  receiver(socket, window_size);
  close(socket);
  return 0;
}
