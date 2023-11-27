#include "../include/simulation.h"
#include "../include/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")
#define enable_cursor() printf("\e[?25h")
#define disable_cursor() printf("\e[?25l")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

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
  int quant_de_indices_para_mostrar = 5;
  int i;
  //Preparando o canvas da simulacao
  for(i = 0; i < quant_de_indices_para_mostrar; i++) printf("\n");

  printf("|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < simulationData->numPoints; i++){
    printf("T==-==-==-==");
  }
  printf("|");
  for(i = 0; i < quant_de_indices_para_mostrar; i++){
    printf("\n|");
    for(int j = 0; j < quant_max_de_pontos_para_mostrar && j < simulationData->numPoints; j++){
      printf("            ");
    }
    printf("|");
  }
  printf("\n|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < simulationData->numPoints; i++){
    printf("-==-==-==-==");
  }
  printf("|");
  //Desenhando passageiros em quais pontos
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < simulationData->numPoints; i++){
    point_queueLock(simulationData->points[i]);
    queue_node *current = point_getQueue(simulationData->points[i])->first_node;
    for(int j = 0; j < quant_de_indices_para_mostrar && j < point_getNumPassengersInQueue(simulationData->points[i]); j++){
      gotoxy(2 + i * 12, quant_de_indices_para_mostrar - j);
      printf("%d", passenger_getID(current->passenger));
      current = current->prev_node;
    }
    point_queueUnlock(simulationData->points[i]);
  }
  //Desenhando onibus em quais pontos
  int* quant_de_onibus_mostrados_no_ponto = calloc(quant_max_de_pontos_para_mostrar, sizeof(int));
  int* quant_de_onibus_mostrados_no_entre_pontos = calloc(quant_max_de_pontos_para_mostrar, sizeof(int));
  for(int j = 0; j < simulationData->numBus; j++){
    BUS *bus = simulationData->buses[j];
    int currentPointID = bus->currentPointID;
    int nextPointID = bus->nextPointID - 1;
    if(nextPointID < 0) nextPointID = simulationData->numPoints - 1;
    if(currentPointID >= 0 && currentPointID < quant_max_de_pontos_para_mostrar 
		    && quant_de_onibus_mostrados_no_ponto[currentPointID] < quant_de_indices_para_mostrar){
      gotoxy(2 + currentPointID * 12, quant_de_indices_para_mostrar + quant_de_onibus_mostrados_no_ponto[currentPointID] + 2);
      printf("%d", j); //j eh o index do onibus

      quant_de_onibus_mostrados_no_ponto[currentPointID]++;
    }
    else if(nextPointID >= 0 && nextPointID < quant_max_de_pontos_para_mostrar 
		    && quant_de_onibus_mostrados_no_entre_pontos[nextPointID] < quant_de_indices_para_mostrar){
      gotoxy(2 + 6 + nextPointID * 12, quant_de_indices_para_mostrar + quant_de_onibus_mostrados_no_entre_pontos[nextPointID] + 2);
      printf("%d", j); //j eh o index do onibus

      quant_de_onibus_mostrados_no_entre_pontos[nextPointID]++;
    }
  }
  free(quant_de_onibus_mostrados_no_ponto);
  free(quant_de_onibus_mostrados_no_entre_pontos);
  
  gotoxy(1,quant_de_indices_para_mostrar * 2 + 3);
  printf("---------- Status da Simulacao ----------\n");
  printf("Quantidade de passageiros restantes: %d\n", simulationData->numPassengersRemaining);
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
