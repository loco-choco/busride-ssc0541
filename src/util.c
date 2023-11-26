#include "../include/util.h"
#include <stdio.h>
#include <time.h>

void string_delete(char **string) {
    if(*string != NULL) {
        free(*string);
        *string = NULL;
    }
}

char *string_getCurrentTime() {
    struct tm *currentTime;     
    time_t seconds;
    time(&seconds);   
    currentTime = localtime(&seconds);  

    char *result = calloc(12, sizeof(char));
    sprintf(result, "%02d:%02d:%02d", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
    
    return result;
}