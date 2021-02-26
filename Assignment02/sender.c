// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"

#define inc(k) if (k < w_size) k = k + 1; else k = 0;

int w_size;
int sockfd;  // socked used for UDP transmission

// Construct and send a data, ack, or nak frame. 
static void send_frame(frame_kind fk, seq_nr frame_nr, 
    seq_nr frame_expected, packet buffer[]) {
  frame f;
  int max_seq;

  max_seq = 2 * w_size - 1;
  printf("Max seq: %d\n", max_seq);

  f.kind = fk;
  f.seq = frame_nr;
  f.ack = (frame_expected + max_seq) % (max_seq + 1);

  if (fk == data)
    strcpy(f.info, buffer[frame_nr % w_size].data);
  write(sockfd, &f, sizeof(f));
}

void sender(int socket, int timeout) {
  // dummy variables for UDP transmission. 
  struct sockaddr_storage addr;
  socklen_t addr_len;

  char input[MSG_LEN];  // store user input

  frame f;  // scratch variable

  // For timeout setup
  int sel_val;
  fd_set read_fd_set, write_fd_set;
  struct timeval tv;

  seq_nr nbuffered;  // how many output buffers currently used
  seq_nr ack_expected;  // Lower edge of the sender's window
  seq_nr next_frame_to_send;  // upper edge of the sender's window + 1
  
  nbuffered = 0;  // initially no packets are buffered
  ack_expected = 0; // next ack expected on the inbound stream
  next_frame_to_send = 0;  // number of next outgoing frame
  addr_len = sizeof(struct sockaddr_storage);

  packet out_buf[w_size];  // buffers for the outbound stream


  for (;;) {
    FD_ZERO(&read_fd_set);
    FD_SET(0, &read_fd_set);
    FD_SET(sockfd, &read_fd_set);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    printf("> ");
    fflush(stdout);  // force the buffered data to written to the screen. 

    /* if (select(sockfd + 1, &read_fd_set, NULL, NULL, &tv)) { */
      if (nbuffered < w_size) {
        nbuffered++;  // Expand the window
        if (fgets(input, sizeof(input), stdin) != 0)  // get user input
          input[strcspn(input, "\n")] = 0;
        // Put current frame to buffer. 
        strcpy(out_buf[next_frame_to_send % w_size].data, input);
        send_frame(data, next_frame_to_send, 0, out_buf);  // transmit the frame
        inc(next_frame_to_send);  // advance upper window edge
      } 
      // wait while the current window is full
      else {
          printf("sender: no new frame will send until ack received\n");
          // Note that frame from receiver is guaranteed a ACK frame
          recvfrom(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, &addr_len);
          printf("%d, %d, %d\n", ack_expected, f.ack, next_frame_to_send);
          while (between(ack_expected, f.ack, next_frame_to_send)) {
            nbuffered--;  // handle piggybacked ack
            inc(ack_expected);  // advance lowe redge of sender's window
        }
      }
    } 
    /* // Time out */
    /* else { */
    /*   printf("\nTime out\n"); */
    /* } */
  /* } */
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
