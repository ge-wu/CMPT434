// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"

boolean no_nak = true;
int window_size;

#define inc(k) if (k < window_size) k = k + 1; else k = 0;

static boolean between(seq_nr a, seq_nr b, seq_nr c) {
  return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}

void receiver(int socket) {
  struct sockaddr_storage addr;
  socklen_t addr_len;
  char buf[MSG_LEN];
  frame f;
  frame s;

  seq_nr too_far;
  seq_nr frame_expected;
  boolean arrived[window_size];

  for (int i = 0; i < window_size; i++) 
    arrived[i] = false;

  frame_expected = 0;
  too_far = window_size;

  for (;;) {
    printf("Cur window range: %d-%d\n", frame_expected, too_far);
    addr_len = sizeof(struct sockaddr_storage);
    recvfrom(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, &addr_len);
    // Parse the message and sequence number from the f. 

    printf("sequence number: %d\nmessage: %s\n", f.seq, f.info);

    // Ask the receiver to input an ack. 
    printf("(R) received message? (Y/N): ");
    if (fgets(buf, sizeof ack, stdin) != NULL) {
      buf[strcspn(buf, "\n")] = 0;
    }

    if (buf[0] == 'Y') {
      f.kind = ack;
      printf("(R) acknowledge for frame %d sent\n", f.seq);
      // TODO: shift window here
      if (between(frame_expected, f.seq, too_far) && !arrived[f.seq % window_size]) {
        arrived[f.seq % window_size] = true;
        while (arrived[frame_expected % window_size]) {
          arrived[frame_expected % window_size] = false;
          // advance lower edge of the receiver's window
          inc(frame_expected);
          // advance upper edge of the receiver's window
          inc(too_far);
        }
      }
    } else {
      f.kind = nak;
      printf("receiver: message not successfully received\n");
    }
    sendto(socket, &f, sizeof f, 0, (struct sockaddr*) &addr, addr_len);
    // Send back the ack to the receiver 
  }
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("usage: ./receiver 30000 <window size>");
    exit(1);
  }

  int socket;

  window_size = atoi(argv[2]);
  
  if (window_size < MIN_WSIZE || window_size > MAX_WSIZE) {
    printf(
        "error: window_size must between %d to %d, inclusive", 
        MIN_WSIZE, MAX_WSIZE);
    exit(1);
  }

  socket = get_udp_server_socket(PORT);

  printf("receiver is now listening...\n");
  receiver(socket);
  close(socket);
  return 0;
}
