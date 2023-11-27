#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include "passenger.h"

typedef struct _node {
  PASSENGER* passenger;
  struct _node* next_node;
  struct _node* prev_node;
} queue_node;

typedef struct _root {
  queue_node* first_node;
  queue_node* last_node;
  int count;
} QUEUE;

QUEUE* queue_create();
int queue_size(QUEUE* queue);
int queue_queue(QUEUE* queue, PASSENGER* element);
PASSENGER *queue_getFist(QUEUE* queue);
int queue_deque(QUEUE* queue);
void queue_free(QUEUE* queue);
#endif