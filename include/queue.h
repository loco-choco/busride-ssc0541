#include <stdlib.h>
#ifndef QUEUE_H
#define QUEUE_H
typedef int element_t;

typedef struct queue_node{
  element_t* element;
  struct queue_node* next_node;
  struct queue_node* prev_node;
} queue_node;

typedef struct queue_root{
  queue_node* first_node;
  queue_node* last_node;
} queue_root;

queue_root* queue_create();
int queue_queue(queue_root* queue, element_t* element);
int queue_deque(queue_root* queue, element_t** dequed_element);
void queue_free(queue_root* queue);
#endif
