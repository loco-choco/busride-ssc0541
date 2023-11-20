#include <stdio.h>
#include <stdlib.h>

#include "../include/bus.h"
#include "../include/passenger.h"
#include "../include/point.h"

void bus_program() {

}

void passenger_program() {

}

void point_program() {

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