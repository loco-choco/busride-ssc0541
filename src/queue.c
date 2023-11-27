#include "../include/queue.h"

QUEUE* queue_create(){
  QUEUE* queue = (QUEUE*) calloc(1, sizeof(QUEUE));
  return queue;
}

int queue_size(QUEUE* queue) {
  if(queue == NULL) return -1;
  return queue->count;
}

int queue_queue(QUEUE* queue, PASSENGER* passenger){
  if(queue == NULL) return 0;
  queue_node* node = (queue_node*) malloc(sizeof(queue_node));
  if(node == NULL) return 0;
  node->passenger = passenger;
  node->next_node = queue->last_node;
  node->prev_node = NULL;
  queue->count++;

  if(queue->last_node != NULL) queue->last_node->prev_node = node;
  queue->last_node = node;

  if(queue->first_node == NULL) queue->first_node = node;

  return 1;
}

PASSENGER *queue_getFist(QUEUE* queue){
    if(queue == NULL) return NULL;
    if(queue->first_node == NULL) return NULL;
    return queue->first_node->passenger;
}



int queue_deque(QUEUE* queue) {
  if(queue == NULL) return -2;
  if(queue->first_node == NULL){
    //*dequed_passenger = NULL;
    return -1;
  }
  queue_node* node = queue->first_node;
  //*dequed_passenger = node->passenger;
  
  queue->first_node = node->prev_node;
  queue->count--;
  if(queue->first_node != NULL) queue->first_node->next_node = NULL;
  
  if(queue->last_node == node) queue->last_node = NULL;
  
  free(node);
  return 1;
}

void queue_free(QUEUE *queue){
  if(queue != NULL){
    queue_node* node = queue->last_node;
    while(node != NULL){
      queue_node* next_node = node->next_node;
      free(node);
      node = next_node;
    }
    free(queue);
  }
}