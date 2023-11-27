#include "../include/bus.h"
#include "../include/passenger.h"
#include "../include/point.h"
#include "../include/simulation.h"
#include <stdio.h>

void *passenger_program(ARGS *args) {
    SIMULATION *simulationData = args->simulation;
    int myID = args->myId;

    PASSENGER *passenger = simulationData->passengers[myID];

    passenger_setSoucePointID(passenger, rand() % simulationData->numPoints);
    passenger_setDestinationPointID(passenger, rand() % simulationData->numPoints);
    
    POINT *sourcePoint = simulationData->points[passenger_getSourcePointID(passenger)];
    POINT *destinationPoint = simulationData->points[passenger_getDestinationPointID(passenger)];
    
    //VAI PARA O PONTO INICIAL
    point_queueLock(sourcePoint);
    point_queue(sourcePoint, passenger);
    passenger_setTimeArrival(passenger, string_getCurrentTime());

    //Espera onibus
    while(!point_hasBus(sourcePoint) || !point_hasSpaceAvaliableInBus(sourcePoint)) {
        point_waitForBus(sourcePoint, passenger);
    }

    //Sai do ponto e Entra no onibus

    BUS *bus = point_getBus(sourcePoint);
    passenger_setBus(passenger, bus);

    pthread_mutex_lock(&(bus->passengersMutex));
    point_deque(sourcePoint);

    bus->numPassengers += 1;
    passenger_setTimeBoarding(passenger, string_getCurrentTime());
   
    pthread_mutex_unlock(&(bus->passengersMutex));
    point_queueUnlock(sourcePoint);
    pthread_cond_signal(&(bus->bus_th));

    //confere e dorme no onibus
    pthread_mutex_lock(&(bus->countMutex));
    while(point_getBus(destinationPoint) != bus) {
        pthread_cond_wait(&(bus->passengers_th), &(bus->countMutex));
        pthread_cond_signal(&(bus->bus_th));
        bus->count -= 1;
    }
    
    //sai do onibus;
    pthread_mutex_lock(&(bus->passengersMutex));
    bus->numPassengers -= 1;
    passenger_setTimeDisembarkation(passenger, string_getCurrentTime());
    pthread_mutex_unlock(&(bus->passengersMutex));
    pthread_mutex_unlock(&(bus->countMutex));

    //salvar arquivo:
    passenger_saveFile(passenger);

    args_erase(&args);
    simulationData->numPassengersRemaining -= 1; 
    pthread_exit(0);
}