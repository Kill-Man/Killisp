#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int ind_blk, tab_val, singleqt_str, doubleqt_str, whitespc;

void parser_init(int indent_block, int tab_value, int singlequote_str, int doublequote_str, int whitespace)
{
    ind_blk = indent_block;
    tab_val = tab_value;
    singleqt_str = singlequote_str;
    doubleqt_str = doublequote_str;
    whitespc = whitespace;
}

/* !!!!!DO LATER (AFTER DONE WITH LANGUAGE)!!!!!
void div_indiv_lines(char ***indiv_lines, const char *string, const char *error, const int error_code)
{
    int line_pos = 0;
    printf("Stage 1\n");
    int indiv_lines_line = 0;
    printf("Stage 2\n");
    *indiv_lines = (char **) malloc(sizeof(char *));
    printf("Stage 3\n");
    if (*indiv_lines == NULL) {
        puts(error);
        exit(error_code);
    }
    printf("Stage 4\n");
    **indiv_lines = (char *) malloc(sizeof(char));
    printf("Stage 5\n");
    if (**indiv_lines == NULL) {
        puts(error);
        exit(error_code);

    printf("Stage 6\n");
    char *line = (char *) malloc(sizeof(char));
    printf("Stage 7\n");
    for (int i = 0; *(string + i) != '\0'; i++) {
        line = (char *) realloc(line, sizeof(char) * (line_pos + 1));
        printf("Stage 8.1\n");
        if (line == NULL) {
            puts(error);
            exit(error_code);
        }
        printf("Stage 8.2\n");
        *(*indiv_lines + indiv_lines_line) = (char *) realloc(*(*indiv_lines + indiv_lines_line), sizeof(char) * (line_pos + 1));
        printf("Stage 8.3\n");
        if (line == NULL) {
            puts(error);
            exit(error_code);
        }
        printf("Stage 8.4\n");
        if (*(string + i) == '\n') {
            *(line + line_pos) = '\0';
            printf("Stage 8.5.1\n");
            line_pos = 0;
            printf("Stage 8.5.2\n");
            indiv_lines_line++;
            printf("Stage 8.5.3\n");
            strcpy(*(*indiv_lines + indiv_lines_line), line);
            printf("Stage 8.5.4\n");
            free(line); /////////////////////////////////////////////////////////////////////// <--THIS IS WHERE THE ERROR IS FIX IT //////////////////////////////////////////////////
            printf("Stage 8.5.5\n");
        } else {
            *(line + line_pos) = *(string + i);
            printf("Stage 8.6.1\n");
            line_pos++;
            printf("Stage 8.6.2\n");
        }
    }
    *(*(*indiv_lines + indiv_lines_line) + line_pos) = '\0';
    printf("Stage 9\n");
    free(line);
    printf("Stage 10\n");
}
*/

void tokenize(char ***token_set, const char *token_str, const char *error, const int error_code)
{
    int i;
    for (i = 0; ; i++) {
        if (*(*token_set + i) == NULL) {
            free(*(*token_set + i));
            break;
        } else {
            free(*(*token_set + i));
        }
    }
    free(*token_set);
    *token_set = (char **) malloc (sizeof(char *));
    char *token = (char *) malloc(sizeof(char)); // string to store current token
    int token_loc = 0; // int to know where in token the parser is
    int current_token = 0; // int to know where in token_set to store the value of *token
    int token_str_loc = 0; // int to know where in token_str the parser is
    char in_string = 0; // char to know if in string

    if (ind_blk || whitespc) {
        // do later
    } else { // if all whitespace is ignored
        while (isspace(*(token_str + token_str_loc))) { // ignore all whitespace
            token_str_loc++; // move in token_str until non-whitespace character
        }
    }

    while (*(token_str + token_str_loc) != '\0' && *(token_str + token_str_loc) != EOF) {
        free(token);
        token = (char *) malloc(sizeof(char));
        if (whitespc) {
            // do later
        } else {
            while (isspace(*(token_str + token_str_loc))) {
                token_str_loc++;
            }
        }

        if (isalpha(*(token_str + token_str_loc)) || *(token_str + token_str_loc) == '_') { // if next set of characters starts with a letter
            while (isalnum(*(token_str + token_str_loc)) || *(token_str + token_str_loc) == '_') {
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (isdigit(*(token_str + token_str_loc))) {
            while (isdigit(*(token_str + token_str_loc))) {
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (ispunct(*(token_str + token_str_loc))) {
            token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
            *(token + token_loc) = *(token_str + token_str_loc);
            token_loc++;
            token_str_loc++;
        }

        if (strcmp(token, "")) {
            *(token + token_loc) = '\0';
            *token_set = (char **) realloc(*token_set, sizeof(char *) * (current_token + 2));
            *(*token_set + current_token) = (char *) malloc(sizeof(char) * strlen(token));
            strcpy(*(*token_set + current_token), token);
            token_loc = 0;
            current_token++;
        }
    }
    *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
    *(*token_set + current_token) = NULL;
    free(token);
}
