BIN = ./bin
OBJ = ./obj
INCLUDE = ./include
SRC = ./src
LIB = ./lib

all:
	gcc -c $(LIB)/queue.c -I $(INCLUDE)/ -o $(OBJ)/queue.o
	gcc -c $(SRC)/onibus.c -I $(INCLUDE)/ -o $(OBJ)/onibus.o -pthread
	gcc -c $(SRC)/ponto.c -I $(INCLUDE)/ -o $(OBJ)/ponto.o -pthread
	gcc -c $(SRC)/passageiro.c -I $(INCLUDE)/ -o $(OBJ)/passageiro.o -pthread
	gcc $(SRC)/main.c $(OBJ)/onibus.o $(OBJ)/ponto.o $(OBJ)/passageiro.o $(OBJ)/queue.o -I $(INCLUDE)/ -o $(BIN)/bus-ride -pthread -D DRAW_SIMULATION

simple:
	gcc -c $(LIB)/queue.c -I $(INCLUDE)/ -o $(OBJ)/queue.o 
	gcc -c $(SRC)/onibus.c -I $(INCLUDE)/ -o $(OBJ)/onibus.o -pthread 
	gcc -c $(SRC)/ponto.c -I $(INCLUDE)/ -o $(OBJ)/ponto.o -pthread
	gcc -c $(SRC)/passageiro.c -I $(INCLUDE)/ -o $(OBJ)/passageiro.o -pthread
	gcc $(SRC)/main.c $(OBJ)/onibus.o $(OBJ)/ponto.o $(OBJ)/passageiro.o $(OBJ)/queue.o -I $(INCLUDE)/ -o $(BIN)/bus-ride -pthread 

debug:
	gcc -c $(LIB)/queue.c -I $(INCLUDE)/ -o $(OBJ)/queue.o -D DEBUG 
	gcc -c $(SRC)/onibus.c -I $(INCLUDE)/ -o $(OBJ)/onibus.o -pthread -D DEBUG 
	gcc -c $(SRC)/ponto.c -I $(INCLUDE)/ -o $(OBJ)/ponto.o -pthread -D DEBUG 
	gcc -c $(SRC)/passageiro.c -I $(INCLUDE)/ -o $(OBJ)/passageiro.o -pthread -D DEBUG 
	gcc $(SRC)/main.c $(OBJ)/onibus.o $(OBJ)/ponto.o $(OBJ)/passageiro.o $(OBJ)/queue.o -I $(INCLUDE)/ -o $(BIN)/bus-ride -pthread -D DEBUG 
run:
	$(BIN)/bus-ride

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*
