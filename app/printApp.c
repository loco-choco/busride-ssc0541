#include "../include/simulation.h"
#include "../include/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define clear() printf("\033[H\033[J")
#define enable_cursor() printf("\e[?25h")
#define disable_cursor() printf("\e[?25l")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

void desenhar_status_no_terminal(SIMULATION *simulalationData);
void desenhar_simulacao_no_terminal(SIMULATION *simulalationData);

void *print_program(SIMULATION *simulation) {
    while(simulation->running) {
        clear();
        desenhar_simulacao_no_terminal(simulation);
        //desenhar_status_no_terminal(simulation);
        printf("\n");
        sleep(1);
    }
}

void desenhar_status_no_terminal(SIMULATION *simulalationData){
  int i;
  printf("Quantidade de passageiros restantes: %d\n", simulalationData->numPassengersRemaining);
  printf("Onibus:\n");
  for(i = 0; i < simulalationData->numBus; i++){
    BUS *bus = simulalationData->buses[i];
    printf("\t[%d] - %d passageiros - ponto %d\n", i, bus->numPassengers, bus->currentPointID);
  }
  printf("Pontos:\n");
  for(i = 0; i < simulalationData->numPoints; i++){
    POINT *point = simulalationData->points[i];
    printf("\t[%d] - quantidade de passageiros no ponto %d\n", i, point_getNumPassengersInQueue(point));
  }
  printf("Passageiros:\n");
  for(i = 0; i < simulalationData->numPassengers; i++){
    PASSENGER *passenger = simulalationData->passengers[i];
    int srcID = passenger_getSourcePointID(passenger);
    int destID = passenger_getDestinationPointID(passenger);
    BUS *bus = passenger_getBus(passenger);
    if(bus == NULL){
        printf("\t[%d] - (%d -> %d)\n", i,
                srcID, destID);
    }
    else {
        printf("\t[%d] - (%d -> %d) - no onibus %d - acabou %d\n", i,
		    srcID, destID,
		    bus->id,
		    -1);
    }
  }
  fflush(0);
}

void desenhar_simulacao_no_terminal(SIMULATION *simulalationData){
  int quant_max_de_pontos_para_mostrar = 10;
  int quant_de_indices_para_mostrar = 5;
  int i;
  //Preparando o canvas da simulacao
  for(i = 0; i < quant_de_indices_para_mostrar; i++) printf("\n");

  printf("|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < simulalationData->numPoints; i++){
    printf("T==-==-==-==");
  }
  printf("|");
  for(i = 0; i < quant_de_indices_para_mostrar; i++){
    printf("\n|");
    for(int j = 0; j < quant_max_de_pontos_para_mostrar && j < simulalationData->numPoints; j++){
      printf("            ");
    }
    printf("|");
  }
  printf("\n|");
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < simulalationData->numPoints; i++){
    printf("-==-==-==-==");
  }
  printf("|");
  //Desenhando passageiros em quais pontos
  for(i = 0; i < quant_max_de_pontos_para_mostrar && i < simulalationData->numPoints; i++){
    queue_node *current = point_getQueue(simulalationData->points[i])->first_node;
    for(int j = 0; j < quant_de_indices_para_mostrar && j < point_getNumPassengersInQueue(simulalationData->points[i]); j++){
      gotoxy(2 + i * 12, quant_de_indices_para_mostrar - j);
      printf("%d", passenger_getID(current->passenger));
      current = current->prev_node;
    }
  }
  //Desenhando onibus em quais pontos
  int quant_de_onibus_mostrados_no_ponto[quant_max_de_pontos_para_mostrar];
  for(int j = 0; j < simulalationData->numBus; j++){
    BUS *bus = simulalationData->buses[j];
    if(bus->currentPointID >= 0 && bus->currentPointID < quant_max_de_pontos_para_mostrar 
		    && quant_de_onibus_mostrados_no_ponto[bus->currentPointID] < quant_de_indices_para_mostrar){
      
      gotoxy(2 + bus->currentPointID * 12, quant_de_indices_para_mostrar + quant_de_onibus_mostrados_no_ponto[bus->currentPointID] + 2);
      printf("%d", j); //j eh o index do onibus

      quant_de_onibus_mostrados_no_ponto[bus->currentPointID]++;
    }
  }
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