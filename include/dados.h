#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef DADOS_H
#define DADOS_H

typedef struct ponto_dado{
  int* passageiros_na_fila; //TODO Trocar por Lista FIFO
  int onibus_no_ponto;
  pthread_mutex_t onibus_no_ponto_lock;
  pthread_cond_t cond_onibus_pronto;
  pthread_cond_t cond_onibus_chegou_no_ponto;
} ponto_dado; 

typedef struct onibus_dado{
  int ponto_atual;
  int capacidade;
  int numero_de_passageiros;
  pthread_cond_t cond_chegou_em_ponto;
} onibus_dado; 

extern onibus_dado* onibus_dados;
extern ponto_dado* pontos_dados;
extern int num_passageiros_restantes;
extern int num_pontos;
extern pthread_mutex_t dados_lock;

void onibus();
void ponto();
#endif
