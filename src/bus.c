#include "../include/bus.h"
#include <stdlib.h>


BUS *bus_create(int id, int passengersMax) {
    BUS *bus = calloc(1, sizeof(BUS));
    if(bus != NULL) {
        bus->id = id;

        bus->count = 0;
        bus->maxPassengers = passengersMax;
        bus->numPassengers = 0;

        pthread_mutex_init(&(bus->countMutex), NULL);
        pthread_mutex_init(&(bus->passengersMutex), NULL);
        pthread_cond_init(&(bus->passengers_th), NULL);
    }
    return bus;
}

void bus_erase(BUS **bus) {
    if(*bus != NULL) {
        pthread_mutex_destroy(&((*bus)->countMutex));
        pthread_mutex_destroy(&((*bus)->passengersMutex));
        pthread_cond_destroy(&((*bus)->bus_th));
        pthread_cond_destroy(&((*bus)->passengers_th));
        (*bus)->id = -1; //invalida ID;
        free(*bus); //Libera memória;
        *bus = NULL; //Reseta ponteiro;
    }
}

