#ifndef PASSENGER_H
#define PASSENGER_H

#include "util.h"

typedef struct _passenger PASSENGER;

PASSENGER *passenger_create(int id, int sourceID, int destinationID);

int passenger_getID(PASSENGER *passenger);
int passenger_getSourceID(PASSENGER *passenger);
int passenger_getDestinationID(PASSENGER *passenger);

void passenger_appendOnFile(PASSENGER *passenger, char *message);

void passenger_delete(PASSENGER **passenger);

#endif