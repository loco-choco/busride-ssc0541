#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "dados.h"

void onibus_inicio(){}
void *onibus(void *p_arg){
  int index = (int) *((int *)p_arg);
  //inicio()
  //while continuar:
  //  if tenta_ponto():
  //    esperar_passageiros()
  //  ir_prox_ponto()
  //  if num_passageiros_restantes == 0:
  //  continuar = false
  onibus_inicio();
  int continuar = 1;
  while(continuar){
    pthread_mutex_lock(&dados_lock);
    int ponto_atual = onibus_dados[index].ponto_atual;
    //if tenta_ponto():
    if(pthread_mutex_trylock(&pontos_dados[ponto_atual].onibus_no_ponto_lock) == 0){
      //esperar_passageiros()
      pthread_cond_signal(&pontos_dados[ponto_atual].cond_onibus_chegou_no_ponto);//sinaliza ao ponto que o onibus chegou
      pthread_cond_wait(&pontos_dados[ponto_atual].cond_onibus_pronto, &dados_lock);

    }
    //ir_prox_ponto
    onibus_dados[index].ponto_atual = (onibus_dados[index].ponto_atual + 1) % num_pontos;
    if(num_passageiros_restantes == 0) continuar = 0;
    pthread_mutex_unlock(&dados_lock);
    //espera da ida ao prox_ponto
    sleep(rand()% 5 + 1);
  }
}

