// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"


int w_size;  // window size
int sockfd;  // socked used for UDP transmission


void sender(int timeout) {
  // dummy variables for UDP transmission
  struct sockaddr_storage addr;
  socklen_t addr_len;

  frame f;  // scratch variable

  // For timeout setup
  struct timeval tv;

  seq_nr nbuffered;  // how many output buffers currently used
  seq_nr ack_expected;  // Lower edge of the sender's window
  seq_nr next_frame_to_send;  // upper edge of the sender's window + 1
  char input[MSG_LEN];  // store user input
  boolean sent[w_size];
  
  nbuffered = 0;  // initially no packets are buffered
  ack_expected = 0; // next ack expected on the inbound stream
  next_frame_to_send = 0;  // number of next outgoing frame
  addr_len = sizeof(struct sockaddr_storage);

  tv.tv_sec = timeout;
  tv.tv_usec = 0;
  
  for (int i = 0; i < w_size; i++) 
    sent[i] = true;
  
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror("setsockopt");
    return;
  }

  while (true) {
    while (nbuffered < w_size) {
      printf("Please enter a message: \n");
      nbuffered++;  // Expand the window
      if (fgets(input, sizeof(input), stdin) != 0)  // get user input
        input[strcspn(input, "\n")] = 0;
      // Wrap up a frame that going to send to the receiver
      f.seq = next_frame_to_send;
      strcpy(f.info, input);
      write(sockfd, &f, sizeof f);
      next_frame_to_send++;
    }  
    int temp = recvfrom(sockfd, &f, sizeof f, 0, (struct sockaddr*) &addr, &addr_len);
    if (temp == -1) {
      printf("sender: timeout, resend frame: %d\n", ack_expected);
      f.seq = ack_expected;
      write(sockfd, &f, sizeof f);
    } else {
      printf("sender: ack: %d received\n", f.seq);
      if (between(ack_expected, f.seq, next_frame_to_send) && sent[f.seq % w_size]) {
        sent[f.seq % w_size] = false;  // remove from the buffer
        while (!sent[ack_expected % w_size]) {
          sent[ack_expected % w_size] = true;
          nbuffered--;  // free window size
          ack_expected++;  // advance lower edge of the window
        }
      }
    }
  }
}

int main(int argc, char * argv[]) {
  int timeout;
  if (argc != 5) {
    printf("usage: ./sender <IP address> 30000 <window size> <time out>\n");
    exit(1);
  }

  w_size = atoi(argv[3]);
  timeout = atoi(argv[4]);

  if (w_size < MIN_WSIZE || w_size > MAX_WSIZE) {
    printf("sender: window size must between %d to %d, inclusive", 
        MIN_WSIZE, MAX_WSIZE);
    exit(1);
  }

  if (timeout <  MIN_TIME || timeout > MAX_TIME) {
    printf("senderr: timeout must between %d to %d, inclusive", 
        MIN_TIME, MAX_TIME);
    exit(1);
  }

  sockfd = get_udp_client_socket(argv[1], argv[2]);
  sender(timeout);
  close(sockfd);
  return 0;
}
