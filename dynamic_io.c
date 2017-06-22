#include "dynamic_io.h"
#include <stdlib.h>
#include <stdio.h>
void dynamic_gets(char **string, const char *error, const int error_code)
{
    int i;
    free(*string); // if string unallocated, this line does nothing, no harm
    *string = (char *) malloc(sizeof(char)); // allocate mem for string
    if (*string == NULL) { // check if allocation fails
        puts(error); // print error
        exit(error_code); // exit with error code
    }
    for (i = 0;; i++) { // no need for condition
        *(*string + i) = getchar(); // get single character
        if (*(*string + i) == '\n' || *(*string + i) == EOF) { // if input ends
            *(*string + i) = '\0'; // change last character to '\0'
            break; // leave loop
        }
        *string = (char *) realloc(*string, sizeof(char) * (i + 2)); // realloc
        if (*string == NULL) { // if realloc fails
            puts(error); // print error
            exit(error_code); // exit with error code
        }
    }
}
