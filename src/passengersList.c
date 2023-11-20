#include "../include/passengersList.h"
#include <stdlib.h>

typedef struct _node NODE;
struct _node {
    PASSENGER *passenger;
    NODE *next;
    NODE *previous;
};


struct _passengersList {
    int size;
    NODE *head;
    NODE *tail;
};

PASSENGERSLIST *passengersList_create() {
    PASSENGERSLIST *list = calloc(1, sizeof(PASSENGERSLIST));
    if(list != NULL) {
        list->size = 0;
        list->head = NULL;
        list->tail = NULL;
    }
    return list;
}

NODE *node_create(PASSENGER *passenger) {
    NODE *node = calloc(1, sizeof(NODE));
    if(node != NULL) {
        node->passenger = passenger;
    }
    return node;
}

NODE *node_find(PASSENGERSLIST *list, PASSENGER *passenger) {
    NODE *node = list->head;
    do {
        if(node->passenger == passenger) {
            return node;
        } else {
            node = node->next;
        }
    } while(node != NULL && node != list->head);
    return NULL;
}

void node_erase(NODE **node) {
    if(*node != NULL) {
        (*node)->passenger = NULL;
        (*node)->next = NULL;
        (*node)->previous = NULL;

        free(*node);
        *node = NULL;
    }
}

void passengersList_push(PASSENGERSLIST *list, PASSENGER *passenger) {
    NODE *node = node_create(passenger);
    if(list->size == 0) {
        list->head = node;
        list->tail = node;
        node->next = node;
        node->previous = node;
    } else {
        node->next = list->head;
        node->previous = list->tail;
        list->tail->next = node;
        list->head->previous = node;
        list->tail = node;
    }
    list->size += 1;
}

void passengersList_remove(PASSENGERSLIST *list, PASSENGER *passenger) {
    NODE *node = node_find(list, passenger);
    if(node != NULL) {
        if(list->size == 1) {
            list->head = NULL;
            list->tail = NULL;
        } else {
            node->next->previous = node->previous;
            node->previous->next = node->next;
            if(node == list->head) {
                list->head = node->next;    
            } else if(node == list->tail) {
                list->tail = node->previous;
            }
        }
        node_erase(&node);
        list->size -= 1;
    }
}

PASSENGER *passengersList_head(PASSENGERSLIST *list) {
    return (list->size != 0) ? list->head->passenger :  NULL;
}
PASSENGER *passengersList_tail(PASSENGERSLIST *list) {
    return (list->size != 0) ? list->tail->passenger :  NULL;
}
int passengersList_getSize(PASSENGERSLIST *list) {
    return list->size;
}

void passengersList_erase(PASSENGERSLIST **list) {
    if(*list != NULL) {
        while((*list)->size != 0) {
            PASSENGER *passenger = passengersList_head(*list);
            passengersList_remove(*list, passenger);
        }
        free(*list);
        *list = NULL;
    }
}