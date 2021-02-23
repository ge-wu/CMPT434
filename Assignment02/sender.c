// Jiaye Wang jiw561 11231145

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"

#define inc(k) if (k < window_size) k = k + 1; else k = 0;

int window_size;
int sockfd;  // socked used for UDP transmission
boolean no_nak = true;

static boolean between(seq_nr a, seq_nr b, seq_nr c) {
  return ((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c < a));
}

// Construct and send a data, ack, or nak frame. 
static void send_frame(frame_kind fk, seq_nr frame_nr, 
    seq_nr frame_expected, packet buffer[]) {
  frame f;

  f.kind = fk;
  f.seq = frame_nr;
  f.ack = (frame_expected + window_size) % (window_size + 1);

  if (fk == data)
    strcpy(f.info, buffer[frame_nr % window_size].data);
  if (fk == nak)
    no_nak = false;
  write(sockfd, &f, sizeof(f));
}

void sender(int socket, int timeout) {
  char input[MSG_LEN];

  frame f;

  seq_nr nbuffered;
  seq_nr ack_expected;
  seq_nr next_frame_to_send;
  seq_nr frame_expected;
  seq_nr too_far;
  event_type event;

  nbuffered = 0;
  frame_expected = 0;
  next_frame_to_send = 0;
  ack_expected = 0;
  too_far = window_size;
  event = ready;

  boolean arrived[window_size];
  packet out_buf[window_size];
  packet in_buf[window_size];

  for (int i = 0; i< window_size; i++) {
    arrived[i] = false;
  }

  for (;;) {
    if (nbuffered < window_size) {
      // Expand the sender window (NOT SHIFTING)
      nbuffered++; 
      printf("> ");
      if (fgets(input, sizeof(input), stdin) != 0)  {
        input[strcspn(input, "\n")] = 0;
      }
      // Put current frame to buffer. 
      strcpy(out_buf[next_frame_to_send % window_size].data, input);
      // transmit the frame. 
      send_frame(data, next_frame_to_send, frame_expected, out_buf);
      // Advance upper window edge. 
      inc(next_frame_to_send);
    } 
    // wait while the current sender buffer is full. 
    else { 
      printf("sender: no new frame will send until ack received\n");
      read(socket, &f, sizeof(frame));
      // TODO: shift window on the sender side. 
      // Note here only two possible frame kinds the sender can receive
      // from the receiver, ack and nak. Hence, receiver will not send 
      // data kind frame to sender!
      if (f.kind == ack) {
        nbuffered--;
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

  window_size = atoi(argv[3]);
  timeout = atoi(argv[4]);

  sockfd = get_udp_client_socket(argv[1], argv[2]);
  sender(sockfd, timeout);
  close(sockfd);
}
