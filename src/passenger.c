#include "../include/passenger.h"
#include <stdio.h>

#define FILENAME_LENGTH 30

struct _passenger {
    int id;
    int sourceID;
    int destinationID;
    char *fileName;
};

PASSENGER *passenger_create(int id, int sourceID, int destinationID) {
    PASSENGER *passenger = calloc(1, sizeof(PASSENGER));
    if(passenger != NULL) {
        passenger->id = id;
        passenger->sourceID = sourceID;
        passenger->destinationID = destinationID;

        passenger->fileName = calloc(FILENAME_LENGTH, sizeof(char));
        sprintf(passenger->fileName, "passageiro%02d.trace", id);
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

void passenger_appendOnFile(PASSENGER *passenger, char *message) {
    FILE *file = fopen(passenger->fileName, "a\0");
    if(file != NULL) {
        int position = 0;
		while (message[position] != '\0') {
    		fputc(message[position], file);
    		position++;
        }
    }
    fclose(file);
}

void passenger_delete(PASSENGER **passenger) {
    if(*passenger != NULL) {
        (*passenger)->id = -1;
        string_delete(&((*passenger)->fileName));
        free(*passenger);
        *passenger = NULL;
    }
}