BIN = ./bin
OBJ = ./obj
INCLUDE = ./include
SRC = ./src

all:
	gcc -c $(SRC)/onibus.c -I $(INCLUDE)/ -o $(OBJ)/onibus.o -pthread
	gcc -c $(SRC)/ponto.c -I $(INCLUDE)/ -o $(OBJ)/ponto.o -pthread
	gcc -c $(SRC)/passageiro.c -I $(INCLUDE)/ -o $(OBJ)/passageiro.o -pthread
	gcc $(SRC)/main.c $(OBJ)/onibus.o $(OBJ)/ponto.o $(OBJ)/passageiro.o -I $(INCLUDE)/ -o $(BIN)/bus-ride -pthread

run:
	$(BIN)/bus-ride

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*
