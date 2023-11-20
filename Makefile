UTIL = ./src/util.c ./src/bus.c ./src/passenger.c ./src/passengersList.c ./src/point.c
MAIN = ./app/main.c
BINARY = ./bin/executavel

all:
	@gcc -Wall -Wno-unused-result -Werror -g $(UTIL) $(MAIN) -o $(BINARY) -lm -lpthread

run:
	@$(BINARY)

ex:
	@./$(BINARY) < in/2.in

debug:
	@gcc -DDEBUG -Wall $(MAIN) $(UTIL) -o $(BINARY) -lm

valgrind:
	@valgrind -s --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes $(BINARY)

clean:
	@rm *.o

r:
	@clear all run

zip:
	@zip -r bonus.zip *

deleteZip:
	@rm bonus.zip

refreshZip:
	@deleteZip zip
