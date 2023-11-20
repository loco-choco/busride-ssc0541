#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "dados.h"

void passageiro_inicio(){}
void passageiro_entra_no_ponto(int index, int ponto_index){
  //Adicionar o passageiro a lista FIFO do ponto
}
void passageiro_escrever_trace(int index){
  //TODO Escrever no arquivo trace
}
void passageiro(){
  int index;
  int ponto_inicial;
  int ponto_final;
  //inicio()
  //entra_ponto()
  //espera_onibus()
  //while !ponto_certo():
  //  espera_no_onibus()
  //fazer_atividade()
  //num_passageiros_restantes--
  passageiro_inicio();
  pthread_mutex_lock(&dados_lock);
  passageiro_entra_no_ponto(index, ponto_inicial);
  //espera_onibus()
  pthread_cond_wait(&passageiros_dados[index].cond_entrou_no_onibus, &dados_lock);

  int onibus_atual = passageiros_dados[index].onibus_atual;
  //while !ponto_certo():
  //  espera_no_onibus()
  while(onibus_dados[onibus_atual].ponto_atual != ponto_final)
    pthread_cond_wait(&onibus_dados[onibus_atual].cond_chegou_em_ponto, &dados_lock);
  //fazer_atividade()
  passageiro_escrever_trace(index);
  num_passageiros_restantes--;
  pthread_mutex_unlock(&dados_lock);
  //Tempo da atividade
  sleep(rand() % 10 + 1);
}

