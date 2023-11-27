 #include "../include/point.h"
#include <stdlib.h>


struct _point {
    pthread_mutex_t busMutex;
    BUS *bus;

    pthread_mutex_t queueMutex;
    QUEUE *queue;
};

POINT *point_create(int id) {
    POINT *point = calloc(1, sizeof(POINT));
    if(point != NULL) {
        point->bus = NULL;

        pthread_mutex_init(&(point->busMutex), NULL);
        pthread_mutex_init(&(point->queueMutex), NULL);

        point->queue = queue_create();
    }
    return point;
}

//Queue Functions
void point_queueLock(POINT *point) {
    pthread_mutex_lock(&(point->queueMutex));
} 

void point_queueUnlock(POINT *point) {
    pthread_mutex_unlock(&(point->queueMutex));
}

void point_queue_cond_mutex(POINT *point, pthread_cond_t *cond) {
    pthread_cond_wait(cond, &(point->queueMutex));
}

int point_getNumPassengersInQueue(POINT *point) {
    queue_size(point->queue);
}

//Bus Functions
boolean point_hasPassangers(POINT *point) {
    return queue_size(point->queue) > 0;
}

boolean point_tryAttachBus(POINT *point, BUS *bus) {
    int result = pthread_mutex_trylock(&(point->busMutex));
    if(result == 0) point->bus = bus;
    return result == 0;
}
void point_disattachBus(POINT *point) {
    point->bus = NULL;
    pthread_mutex_unlock(&(point->busMutex));
}

void pont_wakeUpFirst(POINT *point) {
    PASSENGER *passenger = queue_getFist(point->queue);
    passenger_cond_signal(passenger);
}

BUS *point_getBus(POINT *point) {
    return point->bus;
}

//Passenger Functions
void point_deque(POINT *point) {
    queue_deque(point->queue);
}

void point_queue(POINT *point,PASSENGER *passenger) {
    queue_queue(point->queue, passenger);
}

void point_waitForBus(POINT *point, PASSENGER *passenger) {
    passenger_cond_wait(passenger, &(point->queueMutex));
}

boolean point_hasBus(POINT *point) {
    return point->bus != NULL;
}

boolean point_hasSpaceAvaliableInBus(POINT *point) {
    return (point->bus != NULL && point->bus->numPassengers < point->bus->maxPassengers);
}

QUEUE *point_getQueue(POINT *point) {
    return point->queue;
}

void point_erase(POINT **point) {
    if(*point != NULL) {
        pthread_mutex_destroy(&(*point)->busMutex);
        pthread_mutex_destroy(&(*point)->queueMutex);
        queue_free((*point)->queue);
        free(*point);
        *point = NULL;
    }
}
