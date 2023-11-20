#ifndef BUS_H
#define BUS_H

#include "passenger.h"

typedef struct _bus BUS;

BUS *bus_create(int id, int passengersMax);

void bus_embark(BUS *bus, PASSENGER *passenger);
void bus_disembark(BUS *bus, PASSENGER *passenger);

int bus_getID(BUS *bus);
int bus_getOccupationSpace(BUS *bus);
int bus_getAvailableSpace(BUS *bus);

void bus_erase(BUS **bus);

#endif