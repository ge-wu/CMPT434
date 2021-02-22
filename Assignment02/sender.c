#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"

#define MSG_LEN 1024

static boolean between(seq_nr a, seq_nr b, seq_nr c) {
  return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}

void sender(int socket, int window_size, int timeout) {
  frame f;
  int ack;
  char buf[MSG_LEN];

  seq_nr nbuffered;
  seq_nr next_frame_to_send;
  seq_nr frame_expected;
  seq_nr base;
  seq_nr too_far;

  nbuffered = 0;
  frame_expected = 0;
  next_frame_to_send = 0;
  too_far = window_size;

  boolean buffer[window_size];
  for (int i = 0; i< window_size; i++) {
    buffer[i] = false;
  }

  /* buffer = create_queue(sizeof(frame)); */
  base = 0;

  for (;;) {
    // 'If the number of messages that have been sent to the receiver and that 
    // are still unacknowledged is less than the maximum sending window size, 
    // the message should also be immediately sent to the receiver. '
    if (nbuffered < window_size) {
      printf("> ");
      if (fgets(buf, sizeof buf, stdin) != 0)  {
        buf[strcspn(buf, "\n")] = 0;
      }
      f.seq = next_frame_to_send;
      strcpy(f.msg, buf);

      write(socket, &f, sizeof(f));
      next_frame_to_send++;
      nbuffered++;
    } else {
      printf("sender: no new frame will send until ack received\n");
      ack = 0;
      read(socket, &ack, sizeof(int));
      // received a ack. 
      if (ack != -1) {
        printf("(S) acknowledge for frame %d received\n", ack);
        // frames may be received in any order. 
        if (between(frame_expected, ack, too_far) && !buffer[ack % window_size]) {
          printf("Frame in range\n");
          buffer[ack & window_size] = true;
          while (buffer[frame_expected % window_size]) {
            frame_expected++;   // advance upper edge of receiver window. 
            too_far++;  // advance lower edge of receiver window. 
          }
        }
        nbuffered--;
      } else {

      }
    }
  }
}

int main(int argc, char * argv[]) {
  int socket;
  int window_size, timeout;
  if (argc != 5) {
    printf("usage: ./sender <IP address> 30000 <window size> <time out>\n");
    printf("    please check README for more usage detail :)");
    exit(1);
  }

  window_size = atoi(argv[3]);
  timeout = atoi(argv[4]);

  socket = get_udp_client_socket(argv[1], argv[2]);
  sender(socket, window_size, timeout);
  close(socket);
}
