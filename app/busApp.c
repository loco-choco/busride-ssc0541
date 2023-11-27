#include "../include/bus.h"
#include "../include/passenger.h"
#include "../include/point.h"
#include "../include/simulation.h"
#include <unistd.h>
#include <stdio.h>

void *bus_program(ARGS *args) {
    SIMULATION *simulationData = args->simulation; 
    int myId = args->myId;

    BUS *bus = (simulationData->buses)[myId];
    bus->currentPointID = NONE;
    bus->nextPointID = rand() % simulationData->numPoints;

    while(simulationData->running) {

        if(bus->currentPointID == NONE) {
            //VIAJANDO
            usleep(rand() % 2000000);

            //TENTANDO ENTRAR NO PONTO
            POINT *destinationPoint = simulationData->points[bus->nextPointID];
            if(point_tryAttachBus(destinationPoint, bus)) {
                bus->currentPointID = bus->nextPointID;
            }

            //ATRIBUINDO PRÓXIMO DESTINO
            bus->nextPointID = (bus->nextPointID + 1) % simulationData->numPoints;
        } 
        
        if(bus->currentPointID != NONE) {
            //DESEMBARCANDO PESSOAS
            bus->count = bus->numPassengers;
            pthread_cond_broadcast(&(bus->passengers_th));
            pthread_mutex_lock(&(bus->countMutex));
            while(bus->count > 0) pthread_cond_wait(&(bus->bus_th), &(bus->countMutex));
            pthread_mutex_unlock(&(bus->countMutex));

            //EMBARCANDO PESSOAS
            POINT *currentPoint = simulationData->points[bus->currentPointID];
 
            point_queueLock(currentPoint);
            pthread_mutex_lock(&(bus->passengersMutex));
            while(point_hasPassangers(currentPoint) && bus->numPassengers < bus->maxPassengers) {
                pont_wakeUpFirst(currentPoint);
                pthread_mutex_unlock(&(bus->passengersMutex));
                point_queue_cond_mutex(currentPoint, &(bus)->bus_th);      
                pthread_mutex_lock(&(bus->passengersMutex));
            }
            pthread_mutex_unlock(&(bus->passengersMutex));
            point_queueUnlock(currentPoint);

            //SAINDO DO PONTO
            bus->currentPointID = NONE;
            point_disattachBus(currentPoint);
        }
    }
    args_erase(&args);
    pthread_exit(0);
}