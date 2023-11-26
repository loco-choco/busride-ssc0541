#ifndef POINT_H
#define POINT_H

#include "bus.h"
#include "passenger.h"
#include "util.h"

typedef struct _point POINT;

POINT *point_create(int id);

//Bus Functions
boolean point_tryAttachBus(POINT *point, BUS *bus);
void point_disattachBus(POINT *point);

//Passengers Functions
void point_passengersLock(POINT *point);
void point_passengersUnlock(POINT *point);

PASSENGER *point_getFirstPassanger(POINT *point);
void point_removeFirstPassenger(POINT *point);
void point_pushPassenger(POINT *point, PASSENGER *passenger);

void point_erase(POINT **point);

#endif