#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "dados.h"

onibus_dado* onibus_dados;
ponto_dado* pontos_dados;
passageiro_dado* passageiros_dados;
int num_passageiros_restantes;
int num_pontos;
pthread_mutex_t dados_lock;

void iniciar_variaveis_de_simulacao(int quant_onibus, int quant_pontos, int quant_passageiros, int capacidade_onibus);
void liberar_variaveis_de_simulacao(int quant_pontos);
void desenhar_status_no_terminal(int quant_onibus, int quant_pontos, int quant_passageiros);
void desenhar_simulacao_no_terminal(int quant_onibus, int quant_pontos, int quant_passageiros);

int main(void) {
  printf("Bus Ride!\n");
  int quant_onibus = 1; 
  int quant_pontos = 3;
  int quant_passageiros = 2;
  int capacidade_onibus = 1;

  pthread_t* onibus_handle = calloc(quant_onibus, sizeof(pthread_t));
  pthread_t* pontos_handle = calloc(quant_pontos, sizeof(pthread_t));
  pthread_t* passageiros_handle = calloc(quant_passageiros, sizeof(pthread_t));
  //Inicializacao das variaveis de simulacao
  
  pthread_mutex_init(&dados_lock, NULL);
  iniciar_variaveis_de_simulacao(quant_onibus, quant_pontos, quant_passageiros, capacidade_onibus);
  //Inicializacao das threads
  int i;
  //Threads dos Onibus
  int* onibus_indexes = malloc(quant_onibus * sizeof(int));
  for(i = 0; i < quant_onibus; i++){
    onibus_indexes[i] = i;
    if(pthread_create(&onibus_handle[i], 0, (void *) onibus, (void *) &onibus_indexes[i]) != 0){
      printf("Erro criando thread do onibus %d! Saindo...\n", i);
      fflush(0);
      exit(0);
    }
  }
  //Threads dos Pontos
  int* pontos_indexes = malloc(quant_pontos * sizeof(int));
  for(i = 0; i < quant_pontos; i++){
    pontos_indexes[i] = i;
    if(pthread_create(&pontos_handle[i], 0, (void *) ponto, (void *) &pontos_indexes[i]) != 0){
      printf("Erro criando thread do ponto %d! Saindo...\n", i);
      fflush(0);
      exit(0);
    }
  }
  //Threads dos Passageiros
  passageiro_args* passageiros_args = malloc(quant_passageiros * sizeof(passageiro_args));
  for(i = 0; i < quant_passageiros; i++){
    passageiros_args[i].index = i;
    passageiros_args[i].ponto_inicial = 1;
    passageiros_args[i].ponto_final = 0;
    if(pthread_create(&passageiros_handle[i], 0, (void *) passageiro, (void *) &passageiros_args[i]) != 0){
      printf("Erro criando thread do passageiro %d! Saindo...\n", i);
      fflush(0);
      exit(0);
    }
  }

  int continuar = 1;
  while(continuar){
    pthread_mutex_lock(&dados_lock);
    #ifndef DEBUG
    #ifdef DRAW_SIMULATION
    desenhar_simulacao_no_terminal(quant_onibus, quant_pontos, quant_passageiros);
    #else
    desenhar_status_no_terminal(quant_onibus, quant_pontos, quant_passageiros);
    #endif
    #endif
    if(num_passageiros_restantes == 0) continuar = 0;
    pthread_mutex_unlock(&dados_lock);
    usleep(500);
  };

  liberar_variaveis_de_simulacao(quant_pontos);
  free(onibus_indexes);
  free(pontos_indexes);
  free(passageiros_args);
  exit(0);
}
void iniciar_variaveis_de_simulacao(int quant_onibus, int quant_pontos, int quant_passageiros, int capacidade_onibus){
  #ifdef DEBUG
  printf("Criando variaveis da simulacao (O:%d, P:%d, PA:%d, A:%d)\n", quant_onibus, quant_pontos, quant_passageiros, capacidade_onibus);
  fflush(0);
  #endif
  int i;
  num_passageiros_restantes = quant_passageiros;
  num_pontos = quant_pontos;
  //Inicia Dados Onibus
  onibus_dados = malloc(quant_onibus * sizeof(onibus_dado));
  for(i = 0; i < quant_onibus; i++){
    onibus_dados[i].ponto_atual = -1;
    onibus_dados[i].capacidade = capacidade_onibus;
    onibus_dados[i].numero_de_passageiros = 0;
    pthread_cond_init(&onibus_dados[i].cond_chegou_em_ponto, NULL);
  }
  //Inicia Dados Passageiros
  passageiros_dados = malloc(quant_passageiros * sizeof(passageiro_dado));
  for(i = 0; i < quant_passageiros; i++){
    passageiros_dados[i].onibus_atual = -1;
    passageiros_dados[i].passageiro_finalizou = 0;
    pthread_cond_init(&passageiros_dados[i].cond_entrou_no_onibus, NULL);
  }
  //Inicia Dados Pontos
  pontos_dados = malloc(quant_pontos * sizeof(ponto_dado));
  for(i = 0; i < quant_pontos; i++){
    pontos_dados[i].passageiros_na_fila = queue_create();
    pontos_dados[i].onibus_no_ponto = -1;
    pthread_mutex_init(&pontos_dados[i].onibus_no_ponto_lock, NULL);
    pthread_cond_init(&pontos_dados[i].cond_onibus_pronto, NULL);
    pthread_cond_init(&pontos_dados[i].cond_onibus_chegou_no_ponto, NULL);
  }
}
void liberar_variaveis_de_simulacao(int quant_pontos){
  #ifdef DEBUG
  printf("Liberando variaveis de simulacao\n");
  fflush(0);
  #endif
  free(onibus_dados);
  free(passageiros_dados);
  pontos_dados = malloc(quant_pontos * sizeof(ponto_dado));
  for(int i = 0; i < quant_pontos; i++)
    queue_free(pontos_dados[i].passageiros_na_fila);
  free(pontos_dados);
}

void desenhar_status_no_terminal(int quant_onibus, int quant_pontos, int quant_passageiros){
  //Limpar o terminal
  printf("\e[1;1H\e[2J");
  fflush(0);
  int i;
  printf("Quantidade de passageiros restantes: %d\n", num_passageiros_restantes);
  printf("Onibus:\n");
  for(i = 0; i < quant_onibus; i++){
    printf("\t[%d] - %d passageiros - ponto %d\n", i, onibus_dados[i].numero_de_passageiros, onibus_dados[i].ponto_atual);
  }
  printf("Pontos:\n");
  for(i = 0; i < quant_pontos; i++){
    printf("\t[%d] - quantidade de passageiros no ponto %d\n", i, pontos_dados[i].passageiros_na_fila->count);
  }
  printf("Passageiros:\n");
  for(i = 0; i < quant_passageiros; i++){
    printf("\t[%d] - no onibus %d - acabou %d\n", i, passageiros_dados[i].onibus_atual, passageiros_dados[i].passageiro_finalizou);
  }
  fflush(0);
}

void desenhar_simulacao_no_terminal(int quant_onibus, int quant_pontos, int quant_passageiros){
  //Limpar o terminal
  printf("\e[1;1H\e[2J");
  fflush(0);
  int i;
  printf("Quantidade de passageiros restantes: %d\n", num_passageiros_restantes);
  printf("Onibus:\n");
  for(i = 0; i < quant_onibus; i++){
    printf("\t[%d] - %d passageiros - ponto %d\n", i, onibus_dados[i].numero_de_passageiros, onibus_dados[i].ponto_atual);
  }
  printf("Pontos:\n");
  for(i = 0; i < quant_pontos; i++){
    printf("\t[%d] - quantidade de passageiros no ponto %d\n", i, pontos_dados[i].passageiros_na_fila->count);
  }
  printf("Passageiros:\n");
  for(i = 0; i < quant_passageiros; i++){
    printf("\t[%d] - no onibus %d - acabou %d\n", i, passageiros_dados[i].onibus_atual, passageiros_dados[i].passageiro_finalizou);
  }
  fflush(0);
}
