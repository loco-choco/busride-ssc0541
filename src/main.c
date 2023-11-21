#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "dados.h"

#define clear() printf("\033[H\033[J")
#define enable_cursor() printf("\e[?25h")
#define disable_cursor() printf("\e[?25l")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

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
  srand(time(NULL)); //Seed aleatoria
  int quant_onibus = 100; 
  int quant_pontos = 10;
  int quant_passageiros = 1;
  int capacidade_onibus = 100;

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
    passageiros_args[i].ponto_inicial = rand() % quant_pontos;
    passageiros_args[i].ponto_final = rand() % quant_pontos;
    if(pthread_create(&passageiros_handle[i], 0, (void *) passageiro, (void *) &passageiros_args[i]) != 0){
      printf("Erro criando thread do passageiro %d! Saindo...\n", i);
      fflush(0);
      exit(0);
    }
  }

  int continuar = 1;
  disable_cursor(); //Esconde o cursor
  fflush(0);
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
  enable_cursor();
  fflush(0);

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
    passageiros_dados[i].ponto_inicial = -1;
    passageiros_dados[i].ponto_final = -1;
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
  for(int i = 0; i < quant_pontos; i++)
    queue_free(pontos_dados[i].passageiros_na_fila);
  free(pontos_dados);
}

void desenhar_status_no_terminal(int quant_onibus, int quant_pontos, int quant_passageiros){
  //Limpar o terminal
  clear();
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
    printf("\t[%d] - (%d -> %d) - no onibus %d - acabou %d\n", i,
		    passageiros_dados[i].ponto_inicial, passageiros_dados[i].ponto_final,
		    passageiros_dados[i].onibus_atual,
		    passageiros_dados[i].passageiro_finalizou);
  }
  fflush(0);
}

void desenhar_simulacao_no_terminal(int quant_onibus, int quant_pontos, int quant_passageiros){
  //Limpar o terminal
  int quant_max_de_pontos_para_mostrar = 10;
  int quant_de_indices_para_mostrar = 5;
  clear();
  int i;
  //Preparando o canvas da simulacao
  for(i = 0; i < quant_de_indices_para_mostrar; i++) printf("\n");

  printf("|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < quant_pontos; i++){
    printf("T==-==-==-==");
  }
  printf("|");
  for(i = 0; i < quant_de_indices_para_mostrar; i++){
    printf("\n|");
    for(int j = 0; j < quant_max_de_pontos_para_mostrar && j < quant_pontos; j++){
      printf("            ");
    }
    printf("|");
  }
  printf("\n|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < quant_pontos; i++){
    printf("-==-==-==-==");
  }
  printf("|");
  //Desenhando passageiros em quais pontos
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < quant_pontos; i++){
    queue_node* passageiro_esperando = pontos_dados[i].passageiros_na_fila->first_node;
    for(int j = 0; j < quant_de_indices_para_mostrar && j < pontos_dados[i].passageiros_na_fila->count; j++){
      gotoxy(2 + i * 12, quant_de_indices_para_mostrar - j);
      printf("%d", *passageiro_esperando->element);
      passageiro_esperando = passageiro_esperando->prev_node;
    }
  }
  //Desenhando onibus em quais pontos
  int* quant_de_onibus_mostrados_no_ponto = calloc(quant_max_de_pontos_para_mostrar, sizeof(int));
  for(int j = 0; j < quant_onibus; j++){
    onibus_dado dado = onibus_dados[j];
    if(dado.ponto_atual >= 0 && dado.ponto_atual < quant_max_de_pontos_para_mostrar 
		    && quant_de_onibus_mostrados_no_ponto[dado.ponto_atual] < quant_de_indices_para_mostrar){
      
      gotoxy(2 + dado.ponto_atual * 12, quant_de_indices_para_mostrar + quant_de_onibus_mostrados_no_ponto[dado.ponto_atual] + 2);
      printf("%d", j); //j eh o index do onibus

      quant_de_onibus_mostrados_no_ponto[dado.ponto_atual]++;
    }
  }
  free(quant_de_onibus_mostrados_no_ponto);
  /*
  gotoxy(1,quant_de_indices_para_mostrar * 2 + 3);
  printf("---------- Status da Simulacao ----------\n");
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
    printf("\t[%d] - (%d -> %d) - no onibus %d - acabou %d\n", i,
		    passageiros_dados[i].ponto_inicial, passageiros_dados[i].ponto_final,
		    passageiros_dados[i].onibus_atual,
		    passageiros_dados[i].passageiro_finalizou);
  }*/
  fflush(0);
}
