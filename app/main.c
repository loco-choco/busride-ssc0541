#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/bus.h"
#include "../include/passenger.h"
#include "../include/point.h"

#define NONE -1
void bus_program(BUS *bus, POINT **points, int numPoints, boolean *hasPassengerActive) {
    int currentPoint = NONE;
    int nextPoint = 0;
    
    while(*hasPassengerActive) {
        if(currentPoint == NONE) {
            usleep(rand() % 2000); // Percorre a rodovia;

            // Tenta entrar num ponto;
            POINT *destinationPoint = points[nextPoint];
            if(point_tryAttachBus(destinationPoint, bus)) {
                currentPoint = nextPoint;
            }

            nextPoint = nextPoint+1 % numPoints; //seta o próximo ponto;
        } else {
            

            currentPoint = FALSE;
        }
    }
}

void passenger_program() {

}

int main() {
    printf("Iniciando...\n");
    
    int S, C, P, A;
    printf("Pontos de onibus (S): ");
    scanf("%d", &S);
    printf("Onibus (C): ");
    scanf("%d", &C);
    printf("Passageiros (P): ");
    scanf("%d", &P);
    printf("Assentos (A): ");
    scanf("%d", &A);

    if( P < A || A < C || P < C) {
        printf("Valores invalidos!\n");
        return EXIT_FAILURE;
    }

    boolean hasPassengerActive = TRUE;

    POINT *points[S];
    BUS *buses[C];
    PASSENGER *passengers[P];

    pthread_t threads_bus[C];
    pthread_t threads_passengers[P];

    for(int id = 0; id < S; id++) {
        points[id] = point_create(id);
    }

    for(int id = 0; id < C; id++) {
        buses[id] = bus_create(id, A);
        pthread_create(&(threads_bus[id]), NULL, bus_program, );
    }

    for(int id = 0; id < P; id++) {
        int sourceID = rand() % S;
        int destinationID = rand() % S;
        passengers[id] = passenger_create(id, sourceID, destinationID);
        pthread_create(&(threads_passengers[id]), NULL, bus_program, NULL);
    }

    
    for(int id = 0; id < P; id++) pthread_join(threads_passengers, NULL);
    hasPassengerActive = FALSE;
    for(int id = 0; id < C; id++) pthread_join(threads_bus, NULL);;

    printf("Encerrando aplicação...");
    for(int id = 0; id < P; id++) passenger_erase(&passengers[id]);
    for(int id = 0; id < C; id++) bus_erase(&buses[id]);
    for(int id = 0; id < S; id++) point_erase(&points[id]);

    return EXIT_SUCCESS;
}

/* MAIN ALTERNATIVA
int main(int argc, char *argv[]) {
    if(argc < 5) {
        printf("Argumentos insuficientes!\n");
        return EXIT_FAILURE;
    }

    int S = atoi(argv[1]), //Ponto de onibus
        C = atoi(argv[2]), //Onibus
        P = atoi(argv[3]), //Passageiros
        A = atoi(argv[4]); //Assentos

    printf("%d %d %d %d\n", S, C, P, A);
    return EXIT_SUCCESS;
}
*/