#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_io.h"
#include "parse.h"

// test func
void print_qts(const char *str);

const int MAJOR_VERSION = 0;
const int MINOR_VERSION = 0;
const int PATCH_VERSION = 2;

int main(int argc, char **argv)
{
    char  *line = (char *) malloc(sizeof(char)); // String to hold the line from a file or input string.
    char **toks = (char **) malloc(sizeof(char *)); // Array of strings to hold the individual tokens of *line.
    int i;
    *toks = (char *) malloc(sizeof(char));
    *toks = NULL; // needed in order to use tokenize ??
    printf("Killisp v%d.%d.%d\n", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION);
    parser_init(0, 8, 1, 1, '\\', 0); // working on mac/win/lin
    do {
        printf("> ");
        dynamic_gets(&line, "FATAL: Error #1: Unable to allocate memory for line. Exiting.", 1); // working on mac/win/lin
        print_qts(line); // temp line
        //tokenize(&toks, line, "FATAL: Error #2: Unable to allocate memory for token or token_set", 2); // working on mac
        //for (i = 0; *(toks + i); i++) {
        //    puts(*(toks + i));
        //}
    } while (strcmp(line, "(exit)")); // While line != "(exit)"

    free(line);
    for (i = 0; ; i++) {
        if (*(toks + i) == NULL) {
            free(*(toks + i));
            break;
        } else {
            free(*(toks + i));
        }
    }
    free(toks);
    return 0;
}

// test func
void print_qts(const char *str)
{
    int i;
    char in_sqstring, in_dqstring; // chars to know if in string
    in_sqstring = in_dqstring = 0;
    int squote_amnt, dquote_amnt;
    squote_amnt = dquote_amnt = 0;
    int num_bs = 0;
    for (i = 0; i < strlen(str); i++) {
        if (*(str + i) == '\\') {
            num_bs++;
        } else {
            num_bs = 0;
        }
        if (*(str + i) == '\'' && !in_dqstring && !(i > 0 && *(str + i - 1) == '\\') && num_bs % 2 == 0) {
            squote_amnt++;
        } else if (*(str + i) == '\"' && !in_sqstring && num_bs % 2 == 0) {
            if (*(str +i - 1) != '\\') {
                dquote_amnt++;
            }
        }
        in_dqstring = dquote_amnt % 2;
        in_sqstring = squote_amnt % 2;
    }
    char even_sqt, even_dqt;
    even_sqt = squote_amnt % 2 == 0;
    even_dqt = dquote_amnt % 2 == 0;

    printf("There are %d single quotes and %d double quotes that aren't escaped.\n", squote_amnt, dquote_amnt);
    printf("Single quotes # even: %d\n", even_sqt);
    printf("Double quotes # even: %d\n\n", even_dqt);
}
