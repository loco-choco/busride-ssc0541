#include "../include/point.h"
#include "../include/passengersList.h"
#include <stdlib.h>
#include <pthread.h>

struct _point {
    pthread_mutex_t *busMutex;
    BUS *bus;

    pthread_mutex_t *passengersListMutex;
    PASSENGERSLIST *passengersList;
};

POINT *point_create(int id) {
    POINT *point = calloc(1, sizeof(POINT));
    if(point != NULL) {
        point->bus = NULL;

        point->busMutex = calloc(1, sizeof(pthread_mutex_t));
        point->passengersListMutex = calloc(1, sizeof(pthread_mutex_t));

        pthread_mutex_init(point->busMutex, NULL);
        pthread_mutex_init(point->passengersListMutex, NULL);

        point->passengersList = passengersList_create();
    }
    return point;
}

//Bus Functions
boolean point_tryAttachBus(POINT *point, BUS *bus) {
    int result = pthread_mutex_trylock(point->busMutex);
    return result == 0;
}
void point_disattachBus(POINT *point) {
    pthread_mutex_unlock(point->busMutex);
}

//Passenger Functions
void point_passengersLock(POINT *point) {
    pthread_mutex_lock(point->passengersListMutex);
} //PODE SER SUBSTITUIDA

void point_passengersUnlock(POINT *point) {
    pthread_mutex_unlock(point->passengersListMutex);
} //PODE SER SUBSTITUIDA

PASSENGER *point_getFirstPassanger(POINT *point) {
    return passengersList_head(point->passengersList);
}

void point_removeFirstPassenger(POINT *point) {
    PASSENGER *passenger = point_getFirstPassanger(point);
    passengersList_remove(point->passengersList, passenger);
}

void point_pushPassenger(POINT *point, PASSENGER *passenger) {
    passengersList_push(point->passengersList, passenger);
}

int point_getPassengersWaiting(POINT *point) {
    return passengersList_getSize(point->passengersList);
}


void point_erase(POINT **point) {
    if(*point != NULL) {
        pthread_mutex_destroy((*point)->busMutex);
        pthread_mutex_destroy((*point)->passengersListMutex);
        passengersList_erase(&((*point)->passengersList));
        free(*point);
        *point = NULL;
    }
}
