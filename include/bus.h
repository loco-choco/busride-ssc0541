#ifndef BUS_H
#define BUS_H

#include <pthread.h>

#define NONE -1

typedef struct _bus BUS;

struct _bus {
    int id;

    int currentPointID;
    int nextPointID;

    int count;
    int maxPassengers;
    int numPassengers;
    
    pthread_mutex_t countMutex;
    pthread_mutex_t passengersMutex;
    pthread_cond_t passengers_th;
    pthread_cond_t bus_th;
};

BUS *bus_create(int id, int passengersMax);

void bus_erase(BUS **bus);

#endif