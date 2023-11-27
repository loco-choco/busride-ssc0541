#ifndef POINT_H
#define POINT_H

#include "bus.h"
#include "passenger.h"
#include "util.h"
#include "queue.h"
#include <pthread.h>

typedef struct _point POINT;

POINT *point_create(int id);

//Queue Functions
void point_queueLock(POINT *point);
void point_queueUnlock(POINT *point);
void point_queue_cond_mutex(POINT *point, pthread_cond_t *cond);
int point_getNumPassengersInQueue(POINT *point);

//Bus Functions
boolean point_hasPassangers(POINT *point);
boolean point_tryAttachBus(POINT *point, BUS *bus);
void point_disattachBus(POINT *point);
void pont_wakeUpFirst(POINT *point);
BUS *point_getBus(POINT *point);

//Passenger Functions
void point_queue(POINT *point,PASSENGER *passenger);
void point_deque(POINT *point);
void point_waitForBus(POINT *point, PASSENGER *passenger);
boolean point_hasSpaceAvaliableInBus(POINT *point);
boolean point_hasBus(POINT *point);

//Simulation Functions
QUEUE *point_getQueue(POINT *point);

void point_erase(POINT **point);

#endif