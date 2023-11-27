#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>
#include "passenger.h"

typedef struct _root QUEUE;

QUEUE* queue_create();
int queue_size(QUEUE* queue);
int queue_queue(QUEUE* queue, PASSENGER* element);
PASSENGER *queue_getFist(QUEUE* queue);
int queue_deque(QUEUE* queue);
void queue_free(QUEUE* queue);
#endif