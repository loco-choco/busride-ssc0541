#include "../include/bus.h"
#include "../include/passengersList.h"
#include <stdlib.h>
#include <pthread.h>

struct _bus {
    int id;

    int passengersMax;
    pthread_mutex_t *passengersMutex;
    PASSENGERSLIST *passengersList;
};

BUS *bus_create(int id, int passengersMax) {
    BUS *bus = calloc(1, sizeof(BUS));
    if(bus != NULL) {
        bus->id = id;
        bus->passengersMax = passengersMax;
        
        bus->passengersMutex = calloc(1, sizeof(pthread_mutex_t));
        pthread_mutex_init(bus->passengersMutex, NULL);
    }
    return bus;
}

void bus_embark(BUS *bus, PASSENGER *passenger) {
    passengersList_push(bus->passengersList, passenger);
}

void bus_disembark(BUS *bus, PASSENGER *passenger) {
    passengersList_remove(bus->passengersList, passenger);
}

int bus_getID(BUS *bus) {
    return bus->id;
}

int bus_getOccupationSpace(BUS *bus) {
    return passengersList_getSize(bus->passengersList);
}

int bus_getAvailableSpace(BUS *bus) {
    int ocuppation = bus_getOccupationSpace(bus);
    return bus->passengersMax - ocuppation;
}

void bus_erase(BUS **bus) {
    if(*bus != NULL) {
        (*bus)->id = -1; //invalida ID;
        pthread_mutex_destroy((*bus)->passengersMutex); //apaga mutex
        passengersList_erase(&((*bus)->passengersList)); //apaga lista de passageiros
        free(*bus); //Libera memória;
        *bus = NULL; //Reseta ponteiro;
    }
}

