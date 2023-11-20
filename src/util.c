#include "../include/util.h"

void string_delete(char **string) {
    if(*string != NULL) {
        free(*string);
        *string = NULL;
    }
}