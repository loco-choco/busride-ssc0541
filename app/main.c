#include "../include/simulation.h"

#include "passengerApp.c"
#include "busApp.c"
#include "printApp.c"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void read_args(int *S, int *C, int *P, int *A);

int main(void) {
    
    int S, C, P, A;
    read_args(&S, &C, &P, &A);

    /*if( P < A || A < C || P < C) {
        printf("Valores invalidos!\n");
        return EXIT_FAILURE;
    }*/

    POINT *points[S];
    BUS *buses[C];
    PASSENGER *passengers[P];

    SIMULATION simulation;
    simulation.numPoints = S;
    simulation.points = points;
    simulation.numBus = C;
    simulation.buses = buses;
    simulation.numPassengers = P;
    simulation.numPassengersRemaining = P;
    simulation.passengers = passengers;
    simulation.running = TRUE;

    pthread_t threads_bus[C];
    pthread_t threads_passengers[P];
    pthread_t thread_print;

    for(int id = 0; id < S; id++) {
        points[id] = point_create(id);
    }

    for(int id = 0; id < C; id++) {
        buses[id] = bus_create(id, A);
        ARGS *args = args_create(&simulation, id);
        pthread_create(&(threads_bus[id]), NULL, (void *) bus_program, args);
    }

    for(int id = 0; id < P; id++) {
        passengers[id] = passenger_create(id);
        ARGS *args = args_create(&simulation, id);
        pthread_create(&(threads_passengers[id]), NULL, (void *) passenger_program, args);
    }

    pthread_create(&thread_print, NULL, (void *) print_program, &simulation);
    
    for(int id = 0; id < P; id++) pthread_join(threads_passengers[id], NULL);
    simulation.running = FALSE;
    for(int id = 0; id < C; id++) pthread_join(threads_bus[id], NULL);
    pthread_join(thread_print, NULL);

    printf("Encerrando aplicação...\n");
    for(int id = 0; id < P; id++) passenger_erase(&passengers[id]);
    for(int id = 0; id < C; id++) bus_erase(&buses[id]);
    for(int id = 0; id < S; id++) point_erase(&points[id]);

    return EXIT_SUCCESS;
}

void read_args(int *S, int *C, int *P, int *A) {
    printf("Pontos de onibus (S): ");
    scanf("%d", S);
    printf("Onibus (C): ");
    scanf("%d", C);
    printf("Passageiros (P): ");
    scanf("%d", P);
    printf("Assentos (A): ");
    scanf("%d", A);
}