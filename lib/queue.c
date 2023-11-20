#include <stdlib.h>
#include "queue.h"

queue_root* queue_create(){
  queue_root* queue = (queue_root*) calloc(1, sizeof(queue_root));
  return queue;
}

int queue_queue(queue_root* queue, element_t* element){
  if(queue == NULL) return 0;
  queue_node* node = (queue_node*) malloc(sizeof(queue_node));
  if(node == NULL) return 0;
  node->element = element;
  node->next_node = queue->last_node;
  node->prev_node = NULL;

  if(queue->last_node != NULL) queue->last_node->prev_node = node;

  if(queue->first_node == NULL) queue->first_node = node;

  return 1;
}

int queue_deque(queue_root* queue, element_t** dequed_element){
  if(queue == NULL) return 0;
  if(queue->first_node == NULL){
    *dequed_element = NULL;
    return 0;
  }
  queue_node* node = queue->first_node;
  *dequed_element = node->element;
  
  queue->first_node = node->prev_node;
  if(queue->first_node != NULL) queue->first_node->next_node = NULL;
  
  if(queue->last_node == node) queue->last_node = NULL;
  
  free(node);
  return 1;
}

void queue_free(queue_root* queue){
  if(queue != NULL){
    queue_node* node = queue->last_node;
    while(node != NULL){
      queue_node* next_node = node->next_node;
      free(node->element);
      free(node);
      node = next_node;
    }
    free(queue);
  }
}


