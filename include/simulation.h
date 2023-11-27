#ifndef SIMULATION_H
#define SIMULATION_H

#include "bus.h"
#include "point.h"
#include "passenger.h"
#include "util.h"
#include <pthread.h>

typedef struct _simulation {
    boolean running;
    int numPassengers;
    int numPassengersRemaining;
    pthread_mutex_t numPassengersRemaining_lock;
    PASSENGER **passengers;
    int numPoints;
    POINT **points;
    int numBus;
    BUS **buses;
} SIMULATION;

typedef struct _args {
    SIMULATION *simulation;
    int myId;
} ARGS;

ARGS *args_create(SIMULATION *simulation, int id);
void args_erase(ARGS **args);

#endif
