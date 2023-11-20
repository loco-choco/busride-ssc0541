#ifndef PASSENGERSLIST_H
#define PASSENGERSLIST_H

#include "passenger.h"

typedef struct _passengersList PASSENGERSLIST;

PASSENGERSLIST *passengersList_create();

void passengersList_push(PASSENGERSLIST *list, PASSENGER *passenger);
void passengersList_remove(PASSENGERSLIST *list, PASSENGER *passenger);
PASSENGER *passengersList_head(PASSENGERSLIST *list);
PASSENGER *passengersList_tail(PASSENGERSLIST *list);
int passengersList_getSize(PASSENGERSLIST *list);

void passengersList_erase(PASSENGERSLIST **list);



#endif