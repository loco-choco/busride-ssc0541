UTIL = ./src/util.c ./src/bus.c ./src/passenger.c ./src/point.c ./src/simulation.c ./src/queue.c
MAIN = ./app/main.c
BINARY = ./bin/executavel
TEST = ./app/test.c
BIN_TEST = ./bin/test 

# From https://stackoverflow.com/a/14061796
# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

all:
	@gcc -Werror -g $(UTIL) $(MAIN) -o $(BINARY) -lm -lpthread

run:
	@$(BINARY) $(RUN_ARGS)

test_comp:
	@gcc -Wall -Wno-unused-result -Werror -g $(UTIL) $(TEST) -o $(BIN_TEST) -lm -lpthread

test_val:
	@valgrind -s --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes $(BIN_TEST)

ex:
	@./$(BINARY) < in/2.in

debug:
	@gcc -DDEBUG -Wall $(MAIN) $(UTIL) -o $(BINARY) -lm

valgrind:
	@valgrind -s --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all --show-reachable=yes $(BINARY)

clean:
	@rm ./obj/*.o ./bin/*

r:
	@clear all run

zip:
	@zip -r bonus.zip *

deleteZip:
	@rm bonus.zip

refreshZip:
	@deleteZip zip
