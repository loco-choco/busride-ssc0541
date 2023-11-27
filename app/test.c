#include <stdio.h>
#include <stdlib.h>


#include "../include/passenger.h"
#include "../include/queue.h"

int main() {
    int num = 5;
    PASSENGER *passengers[num];
    for(int id = 0; id < num; id++) {
        passengers[id] = passenger_create(id);
    }

    QUEUE *fila = queue_create();

    for(int i = 0; i < num; i++) {
        queue_queue(fila, passengers[i]);
    }
    printf("%d\n", queue_size(fila));

   queue_free(fila);

    for(int id = 0; id < num; id++) {
        passenger_erase(&(passengers[id]));
    }

    return 0;
}