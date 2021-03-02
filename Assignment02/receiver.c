// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"

int w_size, sockfd;

void receiver(int socket) {
  // dummy variables for UDP transmission. 
  struct sockaddr_storage addr;
  socklen_t addr_len;

  char input[MSG_LEN];  // store user input

  frame f;  // scratch variable

  seq_nr too_far;  //  upper edge of the receiver's window + 1
  seq_nr frame_expected;  // lower edge of the receiver's window
  boolean arrived[w_size];  // receiver buffer

  frame_expected = 0;  // frame number expected
  too_far = w_size;  // receiver's upper window + 1
  addr_len = sizeof(struct sockaddr_storage);

  for (int i = 0; i < w_size; i++) 
    arrived[i] = false;

  while (true) {
    // get the frame from sender and output it's sequence number and message. 
    recvfrom(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, &addr_len);
    printf("-----------------------------------------------\n"); // split line
    printf("sequence number: %d\nmessage: %s\n", f.seq, f.info);

    // promote input to simulate whether frame is received or lost. 
    printf("receiver: received message? (Y/N): ");
    if (fgets(input, sizeof input, stdin) != NULL)
      input[strcspn(input, "\n")] = 0;

    // Y for received, send a frame back to the sender. 
    if (input[0] == 'Y') {
      printf("receiver: acknowledge for frame %d sent\n", f.seq);
      // frame can be received in any order.
      if (between(frame_expected, f.seq, too_far) && !arrived[f.seq % w_size]) {
        arrived[f.seq % w_size] = true;  //  mark buffer as full

        // when a new data frame is considered to be correctly received, the 
        // data frame that in front of it maybe already received. Therefore, 
        // we need to shift the lower edge of the receiver's window one by one. 
        // If the lower edge corresponded data frame is received, we will 
        // remove from the buffer or transfer to the next layer. Otherwise, 
        // the receiver window cannot shift forward even data frames that 
        // are already received after the lower edge. 
        while (arrived[frame_expected % w_size]) {
          arrived[frame_expected % w_size] = false;  // remove from the buffer
          frame_expected++;  // advance lower edge of the receiver's window
          too_far++;  // advance upper edge of the receiver's window
        }
        sendto(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, addr_len);
      }
    }
    // do nothing if the input is not start with a Y
  }
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("usage: ./receiver 30000 <window size>");
    exit(1);
  }

  int socket;

  w_size = atoi(argv[2]);
  
  if (w_size < MIN_WSIZE || w_size > MAX_WSIZE) {
    printf("error: window size must between %d to %d, inclusive", 
        MIN_WSIZE, MAX_WSIZE);
    exit(1);
  }

  socket = get_udp_server_socket(PORT);

  printf("receiver is now listening...\n");
  receiver(socket);
  close(socket);
  return 0;
}
