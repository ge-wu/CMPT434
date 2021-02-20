#include <string.h>
#include "queue.h" 

QUEUE * create_queue(size_t memory_size) {
  QUEUE *q = (QUEUE *) malloc(sizeof(QUEUE));
  if (q == NULL) {
    printf("queue: failed to create queue\n");
    exit(1);
  }

  q->size = 0;
  q->memory_size = memory_size;
  q->head = NULL;
  q->tail = NULL;

  return q;
}

void push(QUEUE * q, void * item) {
  if (q == NULL) {
    printf("push error: queue is NULL\n");
    exit(1);
  }

  NODE * new_node = (NODE *) malloc(sizeof(NODE));
  if (new_node == NULL) {
    printf("push error: failed to create new node\n");
    exit(1);
  }
  new_node->item = malloc(q->memory_size);
  new_node->next = NULL;
  memcpy(new_node->item, item, q->memory_size);

  if (q->size == 0) {
    q->head = q->tail = new_node;
  } else {
    q->tail->next = new_node;
    q->tail = new_node;
  }
  q->size++;
}

void pop(QUEUE * q) {
  if (q == NULL || q->size == 0) {
    printf("pop error: queue is null or empty\n");
    exit(1);
  }
  NODE* target = q->head;

  if (q->size == 1) {
    q->head = NULL;
    q->tail = NULL;
  } else {
    q->head = q->head->next;
  }
  free(target->item);
  free(target);
  q->size--;
}

void * front(QUEUE * q) {
  if (q == NULL) {
    printf("front error: queue cannot be NULL\n");
    return NULL;
  }
  if (q->size == 0) {
    return NULL;
  }
  return q->head->item;
}

