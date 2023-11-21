#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "dados.h"

void onibus_inicio(int index){
  onibus_dados[index].ponto_atual = rand() % num_pontos;
}
void *onibus(void *p_arg){
  int index = (int) *((int *)p_arg);
  #ifdef DEBUG
  printf("Onibus %d inicializado\n", index);
  fflush(0);
  #endif
  //inicio()
  //while continuar:
  //  if tenta_ponto():
  //    esperar_passageiros()
  //  ir_prox_ponto()
  //  if num_passageiros_restantes == 0:
  //  continuar = false
  pthread_mutex_lock(&dados_lock);
  onibus_inicio(index);
  pthread_mutex_unlock(&dados_lock);
  int continuar = 1;
  while(continuar){
    pthread_mutex_lock(&dados_lock);
    int ponto_atual = onibus_dados[index].ponto_atual;
    //if tenta_ponto():
    #ifdef DEBUG
    printf("Onibus %d chegando no ponto %d\n", index, ponto_atual);
    fflush(0);
    #endif
    if(pthread_mutex_trylock(&pontos_dados[ponto_atual].onibus_no_ponto_lock) == 0){
      #ifdef DEBUG
      printf("\tOnibus %d esperando para parar no ponto\n", index);
      #endif
      //esperar_passageiros()
      pthread_cond_broadcast(&onibus_dados[index].cond_chegou_em_ponto);
      #ifdef DEBUG
      printf("\tOnibus %d parado no ponto\n", index);
      #endif
      pontos_dados[ponto_atual].onibus_no_ponto = index;
      pthread_cond_signal(&pontos_dados[ponto_atual].cond_onibus_chegou_no_ponto);//sinaliza ao ponto que o onibus chegou
      pthread_cond_wait(&pontos_dados[ponto_atual].cond_onibus_pronto, &dados_lock); //espera o ponto encher o onibus
      pthread_mutex_unlock(&pontos_dados[ponto_atual].onibus_no_ponto_lock);
    }
    //ir_prox_ponto
    onibus_dados[index].ponto_atual = (onibus_dados[index].ponto_atual + 1) % num_pontos;
    #ifdef DEBUG
    printf("Onibus %d indo para o ponto %d\n", index, onibus_dados[index].ponto_atual);
    #endif
    if(num_passageiros_restantes == 0) continuar = 0;
    pthread_mutex_unlock(&dados_lock);
    //espera da ida ao prox_ponto
      //esperar_passageiros()
    usleep(((rand() % 5) / 2 + 1) * 500000);
  }
  #ifdef DEBUG
  printf("Onibus %d terminando\n", index);
  #endif
  pthread_exit(0);
}

