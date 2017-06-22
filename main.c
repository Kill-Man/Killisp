// breaks on "-aeuimebunhidaueitthia" ?

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_io.h"
#include "parse.h"

const int MAJOR_VERSION = 0;
const int MINOR_VERSION = 0;
const int PATCH_VERSION = 1;

int main(int argc, char **argv)
{
    char  *line = (char *) malloc(sizeof(char)); // String to hold the line from a file or input string.
    char **toks = (char **) malloc(sizeof(char *)); // Array of strings to hold the individual tokens of *line.
    *toks = (char *) malloc(sizeof(char));
    *toks = NULL; // needed in order to use tokenize ??
    printf("Killisp v%d.%d.%d\n", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
    do {
        printf("> ");
        dynamic_gets(&line, "FATAL: Error #1: Unable to allocate memory for line. Exiting.", 1); // working on mac/win/lin
        parser_init(0, 8, 1, 1, 0); // working on mac/win/lin
        tokenize(&toks, line, "FATAL: Error #2: Unable to allocate memory for token or token_set", 2); // broken on mac (will compile everywhere else but not work)
        /*
        puts(*toks);
        puts(*(toks + 1));
        puts(*(toks + 2));
        //*/
        //*
        int i;
        for (i = 0; *(toks + i); i++) {
            puts(*(toks + i));
        }
        //*/
        // puts(line);
    } while (strcmp(line, "(exit)")); // While line != "(exit)"

    free(line);
    return 0;
}
