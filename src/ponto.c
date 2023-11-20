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
  #ifdef DEBUG
  printf("Ponto %d enchendo onibus %d\n", index, onibus_index);
  fflush(0);
  #endif
  int passageiros_na_fila = 1;
  while(onibus_dados[onibus_index].numero_de_passageiros < onibus_dados[onibus_index].capacidade 
     && passageiros_na_fila != 0){
    int* passageiro_entrando_index;
    passageiros_na_fila = queue_deque(pontos_dados[index].passageiros_na_fila, &passageiro_entrando_index);
    if(passageiros_na_fila == 1){
      onibus_dados[onibus_index].numero_de_passageiros++;
      passageiros_dados[*passageiro_entrando_index].onibus_atual = onibus_index;
      #ifdef DEBUG
      printf("\tPassageiro %d entrando no onibus %d\n", *passageiro_entrando_index, onibus_index);
      fflush(0);
      #endif
      pthread_cond_signal(&passageiros_dados[*passageiro_entrando_index].cond_entrou_no_onibus);
      free(passageiro_entrando_index); //Liberar, pois pegamos dado da queue
    }
  }
}
//void ponto_libera_onibus();
void* ponto(void *p_arg){
  int index = (int) *((int *)p_arg);
  #ifdef DEBUG
  printf("Ponto %d iniciando\n", index);
  fflush(0);
  #endif
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
    #ifdef DEBUG
    printf("Ponto %d esperando onibus chegar\n", index);
    fflush(0);
    #endif
    //TODO se o ponto entra aqui, ele nao tem como saber que a simulacao acabou, dando deadlock
    //Temos algumas opcoes:
    //  1 - Ignorar, pois eh a thread the ponto, e ela sera jogada fora quando a main terminar
    //  2 - Fazer com que a main faca uma passagem por todos os pontos quando a simulacao acabar, matando essas threads, 
    //  ou chamando cond_onibus_chegou_no_ponto para elas acordarem e terminarem sozinhas
    //  3 - Fazer com que os onibus passem pelos pontos, ate todas terminarem, mas ai eles teriam que esperar todos os passageiros
    //  terminarem E todos os pontos terminarem
    //Sou mais fa da 2, mas a 3 me parece mais "limpa"
    while(pontos_dados[index].onibus_no_ponto == -1) pthread_cond_wait(&pontos_dados[index].cond_onibus_chegou_no_ponto, &dados_lock);
    ponto_enche_onibus(index, pontos_dados[index].onibus_no_ponto);
    //libera_onibus()
    #ifdef DEBUG
    printf("Ponto %d liberando onibus %d\n", index, pontos_dados[index].onibus_no_ponto);
    fflush(0);
    #endif
    pthread_cond_signal(&pontos_dados[index].cond_onibus_pronto);
    pontos_dados[index].onibus_no_ponto = -1;
    if(num_passageiros_restantes == 0) continuar = 0;
    pthread_mutex_unlock(&dados_lock);
  }
  #ifdef DEBUG
  printf("Ponto %d terminando\n", index);
  fflush(0);
  #endif  
  pthread_exit(0);
}

