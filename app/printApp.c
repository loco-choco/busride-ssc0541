#include "../include/simulation.h"
#include "../include/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")
#define enable_cursor() printf("\e[?25h")
#define disable_cursor() printf("\e[?25l")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define YELLOW(string) "\x1b[93m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

void desenhar_status_no_terminal(SIMULATION *simulationData);
void desenhar_simulacao_no_terminal(SIMULATION *simulationData);

void *print_program(SIMULATION *simulation) {
    disable_cursor();
    while(simulation->running) {
        clear();
        desenhar_simulacao_no_terminal(simulation);
        //desenhar_status_no_terminal(simulation);
        //printf("\n");
        usleep(10000);
    }
    enable_cursor();
    pthread_exit(0);
}

void desenhar_status_no_terminal(SIMULATION *simulationData){
  int i;
  printf("Quantidade de passageiros restantes: %d\n", simulationData->numPassengersRemaining);
  printf("Onibus:\n");
  for(i = 0; i < simulationData->numBus; i++){
    BUS *bus = simulationData->buses[i];
    if(bus->currentPointID != -1) printf("\t[%d] - %d passageiros - parado no ponto %d\n", i, bus->numPassengers, bus->currentPointID);
    else printf("\t[%d] - %d passageiros - indo para o ponto %d\n", i, bus->numPassengers, bus->nextPointID);
  }
  printf("Pontos:\n");
  for(i = 0; i < simulationData->numPoints; i++){
    POINT *point = simulationData->points[i];
    printf("\t[%d] - quantidade de passageiros no ponto %d\n", i, point_getNumPassengersInQueue(point));
  }
  printf("Passageiros:\n");
  for(i = 0; i < simulationData->numPassengers; i++){
    PASSENGER *passenger = simulationData->passengers[i];
    int srcID = passenger_getSourcePointID(passenger);
    int destID = passenger_getDestinationPointID(passenger);
    BUS *bus = passenger_getBus(passenger);
    int arrived = passenger_getArrivedAtDestination(passenger);
    if(bus == NULL){
        printf("\t[%d] - (%d -> %d) - esperando no ponto\n", i,
                srcID, destID);
    }
    else {
        printf("\t[%d] - (%d -> %d) - no onibus %d - acabou %d\n", i,
		    srcID, destID,
		    bus->id,
		    arrived);
    }
  }
  fflush(0);
}

void desenhar_simulacao_no_terminal(SIMULATION *simulationData){
  int quant_max_de_pontos_para_mostrar = 10;
  if(quant_max_de_pontos_para_mostrar > simulationData->numPoints) quant_max_de_pontos_para_mostrar = simulationData->numPoints;
  int quant_de_indices_para_mostrar = 5;
  int offset_do_ponto_mostrado = simulationData->numPoints - 1;
  int i;
  //Preparando o canvas da simulacao
  for(i = 0; i < quant_de_indices_para_mostrar; i++) printf("\n");

  printf("|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar; i++){
    printf("T==-==-==-==");
  }
  printf("|");
  for(i = 0; i < quant_de_indices_para_mostrar; i++){
    printf("\n|");
    for(int j = 0; j < quant_max_de_pontos_para_mostrar; j++){
      printf("            ");
    }
    printf("|");
  }
  printf("\n|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar; i++){
    printf("-==-==-==-==");
  }
  printf("|");
  //Desenhando passageiros em quais pontos
  for(i = 0; i < quant_max_de_pontos_para_mostrar; i++){
    int ponto = (i + offset_do_ponto_mostrado) % simulationData->numPoints;
    point_queueLock(simulationData->points[ponto]);
    queue_node *current = point_getQueue(simulationData->points[ponto])->first_node;
    for(int j = 0; j < quant_de_indices_para_mostrar && j < point_getNumPassengersInQueue(simulationData->points[ponto]); j++){
      gotoxy(2 + i * 12, quant_de_indices_para_mostrar - j);
      printf(YELLOW("%d"), passenger_getID(current->passenger));
      current = current->prev_node;
    }
    point_queueUnlock(simulationData->points[ponto]);
  }
  //Desenhando onibus em quais pontos
  int* quant_de_onibus_mostrados_no_ponto = calloc(quant_max_de_pontos_para_mostrar, sizeof(int));
  int* quant_de_onibus_mostrados_no_entre_pontos = calloc(quant_max_de_pontos_para_mostrar, sizeof(int));
  for(int j = 0; j < simulationData->numBus; j++){
    BUS *bus = simulationData->buses[j];
    int currentPointID = bus->currentPointID;
    int currentPointID_view_pos = (currentPointID + offset_do_ponto_mostrado) % simulationData->numPoints; //Para visualizacao
    int nextPointID = bus->nextPointID - 1;
    if(nextPointID < 0) nextPointID = simulationData->numPoints - 1;
    int nextPointID_view_pos = (nextPointID + offset_do_ponto_mostrado) % simulationData->numPoints; //Para visualizacao

    if(currentPointID >= 0 && currentPointID_view_pos < quant_max_de_pontos_para_mostrar 
		    && quant_de_onibus_mostrados_no_ponto[currentPointID_view_pos] < quant_de_indices_para_mostrar){
      gotoxy(2 + currentPointID_view_pos * 12, quant_de_indices_para_mostrar + quant_de_onibus_mostrados_no_ponto[currentPointID_view_pos] + 2);
      if(bus->numPassengers > 0) printf(GREEN("%d"), j); //j eh o index do onibus
      else printf(RED("%d"), j); //j eh o index do onibus

      quant_de_onibus_mostrados_no_ponto[currentPointID_view_pos]++;
    }
    else if(nextPointID >= 0 && nextPointID_view_pos < quant_max_de_pontos_para_mostrar 
		    && quant_de_onibus_mostrados_no_entre_pontos[nextPointID_view_pos] < quant_de_indices_para_mostrar){
      gotoxy(2 + 6 + nextPointID_view_pos * 12, quant_de_indices_para_mostrar + quant_de_onibus_mostrados_no_entre_pontos[nextPointID_view_pos] + 2);
      if(bus->numPassengers > 0) printf(GREEN("%d"), j); //j eh o index do onibus
      else printf(RED("%d"), j); //j eh o index do onibus

      quant_de_onibus_mostrados_no_entre_pontos[nextPointID_view_pos]++;
    }
  }
  free(quant_de_onibus_mostrados_no_ponto);
  free(quant_de_onibus_mostrados_no_entre_pontos);
  //Desenhando o ID de cada ponto
  for(i = 0; i < quant_max_de_pontos_para_mostrar; i++){
    gotoxy(2 + i * 12, quant_de_indices_para_mostrar + 1);
    printf(BLUE("%d"), (i + offset_do_ponto_mostrado)%simulationData->numPoints);
  }

  
  gotoxy(1, quant_de_indices_para_mostrar * 2 + 3);
  printf("---------- Status da Simulacao ----------\n");
  printf("Quantidade de passageiros restantes: %d\n", simulationData->numPassengersRemaining);
  
  printf("---------------- Legenda ----------------\n");
  printf("\t-> Em "YELLOW("amarelo")" - passageiros esperando no ponto\n");
  printf("\t-> Em "BLUE("azul")" - indice do ponto\n");
  printf("\t-> Em "RED("vermelho")" - onibus sem passageiro\n");
  printf("\t-> Em "GREEN("verde")" - onibus com passageiro\n");
  /*
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
