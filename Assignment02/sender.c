#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "queue.h"


void udp_sender(int socket, int window_size, int timeout) {
  QUEUE *q;
  int cur_sequence_num;
  char buf[MSG_LEN];

  q = create_queue(sizeof(FRAME));
  cur_sequence_num = 0;

  for (;;) {
    while (q->size < window_size) {
      FRAME frame;
      printf("> ");
      if (fgets(buf, sizeof buf, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = 0;
      }

      frame.sequence_num = cur_sequence_num;
      strcpy(frame.msg, buf);
      cur_sequence_num++;

      push(q, &frame);
      /* write(socket, &frame, sizeof frame); */
    } 

    while (q->size) {
      FRAME * cur_front;
      cur_front = front(q);
      printf("Cur message: %s, Cur sequence_num: %d\n", cur_front->msg, cur_front->sequence_num);
      pop(q);
    }

    /* printf("sender: window size is full. Please wait for ack.\n"); */
    /* /1* bzero(buf, MSG_LEN); *1/ */
    /* /1* read(socket, buf, sizeof buf); *1/ */

    /* FRAME * temp = front(q); */
    /* pop(q); */
    /* printf("Cur Sequence %d. Window size: %ld\n", temp->sequence_num, q->size); */
    /* printf("Cur message %s\n", temp->msg); */

    /* if (buf[0] != 'Y') { */
    /*   push(q, &temp); */
    /* } */ 
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
  udp_sender(socket, window_size, timeout);
  close(socket);
}
