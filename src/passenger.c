#include "../include/passenger.h"
#include <stdio.h>
#include <pthread.h>

struct _passenger {
    int id;

    int sourceID;
    int currentID;
    int destinationID;

    char *timeArrival;
    char *timeBoarding;
    char *timeDisembarkation;

};

PASSENGER *passenger_create(int id, int sourceID, int destinationID) {
    PASSENGER *passenger = calloc(1, sizeof(PASSENGER));
    if(passenger != NULL) {
        passenger->id = id;

        passenger->sourceID = sourceID;
        passenger->currentID = sourceID;
        passenger->destinationID = destinationID;

        passenger->timeArrival = NULL;
        passenger->timeBoarding = NULL;
        passenger->timeDisembarkation = NULL;
    }
    return passenger;
}

int passenger_getID(PASSENGER *passenger) {
    return passenger->id;
}

int passenger_getSourceID(PASSENGER *passenger) {
    return passenger->sourceID;
}

int passenger_getDestinationID(PASSENGER *passenger) {
    return passenger->destinationID;
}

void passenger_setCurrentPointID(PASSENGER *passenger, int currentPointID) {
    passenger->currentID = currentPointID;
}

boolean passenger_isOnPoint(PASSENGER *passenger) {
    return (passenger->currentID == passenger->destinationID);
}

void passenger_saveFile(PASSENGER *passenger) {

    char *message = calloc(50, sizeof(char));
    char *fileName = calloc(30, sizeof(char));
    
    sprintf(message, "<%s, %s, %s, %d>", passenger->timeArrival, passenger->timeBoarding, passenger->timeDisembarkation, passenger->destinationID);
    sprintf(fileName, "passageiro%02d.trace", passenger->id);
    
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

void passenger_delete(PASSENGER **passenger) {
    if(*passenger != NULL) {
        (*passenger)->id = -1;

        string_delete(&(*passenger)->timeArrival);
        string_delete(&(*passenger)->timeBoarding);
        string_delete(&(*passenger)->timeDisembarkation);

        free(*passenger);
        *passenger = NULL;
    }
}