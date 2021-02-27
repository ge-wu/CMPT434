// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"

#define inc(k) if (k < w_size) k = k + 1; else k = 0;

int w_size;  // window size
int sockfd;  // socked used for UDP transmission


void sender(int socket, int timeout) {
  // dummy variables for UDP transmission
  struct sockaddr_storage addr;
  socklen_t addr_len;

  frame f;  // scratch variable

  // For timeout setup
  int sel_val;
  fd_set read_fds, write_fds;
  struct timeval tv;

  seq_nr nbuffered;  // how many output buffers currently used
  seq_nr ack_expected;  // Lower edge of the sender's window
  seq_nr next_frame_to_send;  // upper edge of the sender's window + 1
  char input[MSG_LEN];  // store user input
  
  nbuffered = 0;  // initially no packets are buffered
  ack_expected = 0; // next ack expected on the inbound stream
  next_frame_to_send = 0;  // number of next outgoing frame
  addr_len = sizeof(struct sockaddr_storage);

  packet out_buf[w_size];  // buffers for the outbound stream


  for (;;) {
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    if (nbuffered < w_size) {
      nbuffered++;  // Expand the window
      printf("Please enter your input: \n");
      if (fgets(input, sizeof(input), stdin) != 0)  // get user input
        input[strcspn(input, "\n")] = 0;
      // Wrap up a frame that going to send to the receiver
      f.seq = f.ack = next_frame_to_send;
      strcpy(f.info, input);
      write(sockfd, &f, sizeof f);

      inc(next_frame_to_send);  // advance upper window edge
    } else {
      // Note that frame from receiver is guaranteed a ack frame
      recvfrom(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, &addr_len);
      printf("sender: ack: %d received\n", f.seq);
      while (f.seq == ack_expected) {
        nbuffered--;  // handle piggybacked ack
        inc(ack_expected);  // advance lowe redge of sender's window
      }
    }
  }
}

int main(int argc, char * argv[]) {
  int timeout;
  if (argc != 5) {
    printf("usage: ./sender <IP address> 30000 <window size> <time out>\n");
    printf("    please check README for more usage detail :)");
    exit(1);
  }

  w_size = atoi(argv[3]);
  timeout = atoi(argv[4]);

  sockfd = get_udp_client_socket(argv[1], argv[2]);
  sender(sockfd, timeout);
  close(sockfd);
}
