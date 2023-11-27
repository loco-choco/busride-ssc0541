#include "../include/passenger.h"
#include "../include/util.h"
#include <stdio.h>
#include <stdlib.h>

struct _passenger {
    int id;

    int sourcePointID;
    int destinationPointID;
    BUS *bus;

    char *timeArrival;
    char *timeBoarding;
    char *timeDisembarkation;

    pthread_cond_t cond_th;
};

PASSENGER *passenger_create(int id) {
    PASSENGER *passenger = calloc(1, sizeof(PASSENGER));
    if(passenger != NULL) {
        passenger->id = id;

        passenger->bus = NULL;
        passenger->sourcePointID = -1;
        passenger->destinationPointID = -1;

        passenger->timeArrival = NULL;
        passenger->timeBoarding = NULL;
        passenger->timeDisembarkation = NULL;

        pthread_cond_init(&(passenger->cond_th), NULL);
    }
    return passenger;
}

void passenger_setSoucePointID(PASSENGER *passenger, int id) {
    passenger->sourcePointID = id;
}

void passenger_setDestinationPointID(PASSENGER *passenger, int id) {
    passenger->destinationPointID = id;
}

void passenger_setBus(PASSENGER *passenger, BUS *bus) {
    passenger->bus = bus;
}

int passenger_getID(PASSENGER *passenger) {
    return passenger->id;
}

int passenger_getSourcePointID(PASSENGER *passenger) {
    return passenger->sourcePointID;
}

int passenger_getDestinationPointID(PASSENGER *passenger) {
    return passenger->destinationPointID;
}

BUS *passenger_getBus(PASSENGER *passenger) {
    return passenger->bus;
}

void passenger_setTimeArrival(PASSENGER *passenger, char *timeArrival) {
    passenger->timeArrival = timeArrival;
}

void passenger_setTimeBoarding(PASSENGER *passenger, char *timeBoarding) {
    passenger->timeBoarding = timeBoarding;
}

void passenger_setTimeDisembarkation(PASSENGER *passenger, char *timeDisembarkation){
    passenger->timeDisembarkation = timeDisembarkation;
}

void passenger_cond_wait(PASSENGER *passenger, pthread_mutex_t *mutex) {
    pthread_cond_wait(&(passenger->cond_th), mutex);
}

void passenger_cond_signal(PASSENGER *passenger) {
    pthread_cond_signal(&(passenger->cond_th));
}

void passenger_saveFile(PASSENGER *passenger) {

    char *message = calloc(50, sizeof(char));
    char *fileName = calloc(70, sizeof(char));
    
    sprintf(message, "<%s, %s, %s, %d>", passenger->timeArrival, passenger->timeBoarding, passenger->timeDisembarkation, passenger->destinationPointID);
    sprintf(fileName, "./traceFiles/passageiro%02d.trace", passenger->id);
    
    FILE *file = fopen(fileName, "w\0");
    if(file != NULL) {
        int position = 0;
		while (message[position] != '\0') {
    		fputc(message[position], file);
    		position++;
        }
    }
    fclose(file);

    string_delete(&fileName);
    string_delete(&message);
}

void passenger_erase(PASSENGER **passenger) {
    if(*passenger != NULL) {
        (*passenger)->id = -1;

        string_delete(&(*passenger)->timeArrival);
        string_delete(&(*passenger)->timeBoarding);
        string_delete(&(*passenger)->timeDisembarkation);

        free(*passenger);
        *passenger = NULL;
    }
}