#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"


void sender(int socket, int window_size, int timeout) {
  QUEUE* buffer;
  FRAME frame;
  int base, ack;
  char buf[MSG_LEN];

  buffer = create_queue(sizeof(FRAME));
  base = 0;

  for (;;) {
    // 'If the number of messages that have been sent to the receiver and that 
    // are still unacknowledged is less than the maximum sending window size, 
    // the message should also be immediately sent to the receiver. '
    while (buffer->size < window_size) {
      printf("> ");
      if (fgets(buf, sizeof buf, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = 0;
      }
      // Wrap into a frame. 
      frame.sequence_num = base;
      strcpy(frame.msg, buf);
      printf("(S) frame %d sent\n", base);

      push(buffer, &frame);
      write(socket, &frame, sizeof frame);
      base++;
    }
    while (buffer->size == window_size) {
      ack = 0;
      read(socket, &ack, sizeof(int));
      FRAME* temp;
      temp = front(buffer);
      if (temp->sequence_num == ack) {
        printf("(S) acknowledge for frame %d received\n", temp->sequence_num);
        pop(buffer);
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
