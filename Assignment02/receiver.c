// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"

int w_size, sockfd;

#define inc(k) if (k < w_size) k = k + 1; else k = 0;

// Construct and send a data, ack, or nak frame. 
static void send_frame(frame_kind fk, seq_nr frame_nr, 
    seq_nr frame_expected, packet buffer[]) {
  frame f;

  f.kind = fk;
  f.seq = frame_nr;
  f.ack = (frame_expected + w_size) % (w_size + 1);

  if (fk == data)
    strcpy(f.info, buffer[frame_nr % w_size].data);
  write(sockfd, &f, sizeof(f));
}

void receiver(int socket) {
  // dummy variables for UDP transmission. 
  struct sockaddr_storage addr;
  socklen_t addr_len;

  char input[MSG_LEN];  // store user input

  frame f;  // scratch variable

  seq_nr too_far;  //  upper edge of the receiver's window + 1
  seq_nr frame_expected;  // lower edge of the receiver's window
  packet in_buf[w_size];  // buffers for the inbound stream
  boolean arrived[w_size];  // inbound bit map

  frame_expected = 0;  // frame number expected
  too_far = w_size;  // receiver's upper window + 1
  addr_len = sizeof(struct sockaddr_storage);

  for (int i = 0; i < w_size; i++) 
    arrived[i] = false;

  for (;;) {
    // get the frame from sender and output it's sequence number and message. 
    recvfrom(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, &addr_len);
    printf("sequence number: %d\nmessage: %s\n", f.seq, f.info);

    // promote input to simulate whether frame is received or lost. 
    printf("receiver: received message? (Y/N): ");
    if (fgets(input, sizeof ack, stdin) != NULL)
      input[strcspn(input, "\n")] = 0;

    // Y for received, send a frame back to the sender. 
    if (input[0] == 'Y') {
      f.kind = ack;
      printf("receiver: acknowledge for frame %d sent\n", f.seq);
      // TODO: shift window here
      if (between(frame_expected, f.seq, too_far) && !arrived[f.seq % w_size]) {
        arrived[f.seq % w_size] = true;  //  mark buffer as full
        /* in_buf[f.seq % w_size] = f.info;  // insert data into buffer */

        // when a new data frame is considered to be correctly received, the 
        // data frame that in front of it maybe already received. Therefore, 
        // we need to shift the lower edge of the receiver's window one by one. 
        // If the lower edge corresponded data frame is received, we will 
        // remove from the buffer or transfer to the next layer. Otherwise, 
        // the receiver window cannot shift forward even data frames that 
        // are already received after the lower edge. 
        while (arrived[frame_expected % w_size]) {
          // remove from the buffer
          arrived[frame_expected % w_size] = false;
          inc(frame_expected);  // advance lower edge of the receiver's window
          inc(too_far);  // advance upper edge of the receiver's window
        }
        sendto(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, addr_len);
      }
    }
    // Do nothing if not received. 
    else {
      continue;
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
