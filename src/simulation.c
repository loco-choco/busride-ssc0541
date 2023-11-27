#include "../include/simulation.h"
#include <stdlib.h>

ARGS *args_create(SIMULATION *simulation, int id) {
    ARGS *args = calloc(1, sizeof(ARGS));
    if(args != NULL) {
        args->simulation = simulation;
        args->myId = id;
    }
    return args;
}

void args_erase(ARGS **args) {
    if(*args != NULL) {
        free(*args);
        *args = NULL;
    }
}


