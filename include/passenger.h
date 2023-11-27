#ifndef PASSENGER_H
#define PASSENGER_H

#include <pthread.h>
#include "bus.h"

typedef struct _passenger PASSENGER;

PASSENGER *passenger_create(int id);

void passenger_setSoucePointID(PASSENGER *passenger, int id);
void passenger_setDestinationPointID(PASSENGER *passenger, int id);
void passenger_setBus(PASSENGER *passenger, BUS *bus);

int passenger_getID(PASSENGER *passenger);
int passenger_getSourcePointID(PASSENGER *passenger);
int passenger_getDestinationPointID(PASSENGER *passenger);
BUS *passenger_getBus(PASSENGER *passenger);
int passenger_getArrivedAtDestination(PASSENGER *passenger);

void passenger_setTimeArrival(PASSENGER *passenger, char *timeArrival);
void passenger_setTimeBoarding(PASSENGER *passenger, char *timeBoarding);
void passenger_setTimeDisembarkation(PASSENGER *passenger, char *timeDisembarkation);
void passenger_setArrivedAtDestination(PASSENGER *passenger, int arrived);

void passenger_cond_wait(PASSENGER *passenger, pthread_mutex_t *mutex);
void passenger_cond_signal(PASSENGER *passenger);

void passenger_saveFile(PASSENGER *passenger);

void passenger_erase(PASSENGER **passenger);

#endif
