#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
  void * item;
  struct NODE * next;
} NODE;

typedef struct QUEUE {
  size_t size;
  size_t memory_size;
  NODE * head;
  NODE * tail;
} QUEUE;


// initialize the queue
QUEUE * create_queue(size_t);

// push an element to the queue
void push(QUEUE *, void *);

// pop the first-in element in the queue
void pop(QUEUE *);

// get the front element of the queue. 
void * front(QUEUE *);

#endif
