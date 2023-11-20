#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "dados.h"

onibus_dado* onibus_dados;
ponto_dado* pontos_dados;
passageiro_dado* passageiros_dados;
int num_passageiros_restantes;
int num_pontos;
pthread_mutex_t dados_lock;


int main(void) {
  printf("Bus Ride!");
}
