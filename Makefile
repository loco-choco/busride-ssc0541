BIN = ./bin
OBJ = ./obj
SRC = ./src

all:
	gcc $(SRC)/main.c -o $(BIN)/bus-ride

run:
	$(BIN)/bus-ride

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*
