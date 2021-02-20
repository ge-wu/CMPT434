#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "network.h"


int main() {
  QUEUE * q = create_queue(sizeof(FRAME));
  if (q->size != 0) {
    printf("Fail to initialize the queue\n");
  }

  FRAME f1;
  f1.sequence_num = 1;
  strcpy(f1.msg, "a");

  FRAME f2;
  f2.sequence_num = 2;
  strcpy(f2.msg, "b");

  FRAME f3;
  f3.sequence_num = 3;
  strcpy(f3.msg, "c");

  push(q, &f1);
  push(q, &f2);
  push(q, &f3);

  printf("push three items to the q: expect size: 3, get: %ld\n", q->size);
  FRAME *cur_front = front(q);
  printf("front element: expect: 1, get: %d\n", cur_front->sequence_num);
  printf("front element: expect: a, get: %s\n", cur_front->msg);

  pop(q);
  cur_front = front(q);
  printf("pop one element: expect size: 2, get: %ld\n", q->size);
  printf("front element: expect: 2, get: %d\n", cur_front->sequence_num);
  printf("front element: expect: b, get: %s\n", cur_front->msg);

  pop(q);
  cur_front = front(q);
  printf("front element: expect: 3, get: %d\n", cur_front->sequence_num);
  printf("front element: expect: c, get: %s\n", cur_front->msg);

  printf("expect size: 1, get: %ld\n", q->size);

  printf("Test completed\n");
}
