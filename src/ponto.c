#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "dados.h"

void ponto_inicio(){}
//void ponto_espera_onibus();
void ponto_enche_onibus(int index, int onibus_index){
  //while (passageiros_na_fila.lenght > 0 && onibus.numero_passageiros < onibus.capacidade){
  //  onibus.numero_passageiros++;
  //  passageiro_entrando = passageiros_na_fila.deque()
  //  passageiro_entrando.onibus_index = onibus_index;
  //  pthread_cond_signal(&passageiro_entrando.cond_entrou_no_onibus);
  //}
}
//void ponto_libera_onibus();
void* ponto(void *p_arg){
  int index = (int) *((int *)p_arg);
  //inicio()
  //while continuar:
  //  espera_onibus()
  //  enche_onibus()
  //  libera_onibus()
  //  if num_passageiros_restantes == 0:
  //    continuar = false
  ponto_inicio();
  int continuar = 1;
  while(continuar){
    pthread_mutex_lock(&dados_lock);
    //espera_onibus()
    pthread_cond_wait(&pontos_dados[index].cond_onibus_chegou_no_ponto, &dados_lock);
    ponto_enche_onibus(index, pontos_dados[index].onibus_no_ponto);
    //libera_onibus()
    pthread_cond_signal(&pontos_dados[index].cond_onibus_pronto);
    if(num_passageiros_restantes == 0) continuar = 0;
    pthread_mutex_unlock(&dados_lock);
  }
}

