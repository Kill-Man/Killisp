#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int ind_blk, tab_val, singleqt_str, doubleqt_str, last_qt_str, whitespc;
static char esc_char;

void parser_init(int indent_block, int tab_value, int singlequote_str, int doublequote_str, char escape_character, int last_quote_str, int whitespace)
{
    ind_blk      = indent_block;
    tab_val      = tab_value;
    singleqt_str = singlequote_str;
    doubleqt_str = doublequote_str;
    esc_char     = escape_character;
    last_qt_str  = last_quote_str;
    whitespc     = whitespace;
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

    // freeing token_set
    for (i = 0; ; i++) {
        if (*(*token_set + i) == NULL) {
            free(*(*token_set + i));
            break;
        } else {
            free(*(*token_set + i));
        }
    }
    free(*token_set);

    // allocating token_set
    *token_set = (char **) malloc (sizeof(char *));

    // allocating token
    char *token = (char *) malloc(sizeof(char)); // string to store current token

    // position variables
    int token_loc = 0; // int to know where in token the parser is
    int current_token = 0; // int to know where in token_set to store the value of *token
    int token_str_loc = 0; // int to know where in token_str the parser is

    // count non-escaped quotes in string
    char in_sqstring, in_dqstring; // chars to know if in string
    in_sqstring = in_dqstring = 0;
    int squote_amnt, dquote_amnt;
    squote_amnt = dquote_amnt = 0;
    int num_esc = 0; // holds number of esc in a row before quote (if even, quote ends)
    int num_esc_temp;
    for (i = 0; i < strlen(token_str); i++) {
        if (*(token_str + i) == esc_char) {
            num_esc++;
            num_esc_temp = num_esc;
        } else {
            num_esc_temp = num_esc;
            num_esc = 0;
        }
        if (*(token_str + i) == '\'' && !in_dqstring && num_esc_temp % 2 == 0) {
            squote_amnt++;
        } else if (*(token_str + i) == '\"' && !in_sqstring && num_esc_temp % 2 == 0) {
            dquote_amnt++;
        }
        in_dqstring = dquote_amnt % 2;
        in_sqstring = squote_amnt % 2;
    }
    num_esc = 0;
    char even_sqt, even_dqt;
    even_sqt = squote_amnt % 2 == 0;
    even_dqt = dquote_amnt % 2 == 0;

    // code for whitespc testing at beginning of line
    if (ind_blk || whitespc) {
        // do later
    } else { // if all whitespace is ignored
        while (isspace(*(token_str + token_str_loc))) { // ignore all whitespace
            token_str_loc++; // move in token_str until non-whitespace character
        }
    }

    // token loop
    while (*(token_str + token_str_loc) != '\0' && *(token_str + token_str_loc) != EOF) { // while not the end of the line
        // get token prepared
        free(token);
        token = (char *) malloc(sizeof(char));

        // code for if whitespc is enabled
        if (whitespc) {
            // do later
        } else {
            while (isspace(*(token_str + token_str_loc))) {
                token_str_loc++;
            }
        }

        // if / else if / else for placing parsed string into token
        if (isalpha(*(token_str + token_str_loc)) || *(token_str + token_str_loc) == '_') { // if next set of characters starts with a letter or number
            while (isalnum(*(token_str + token_str_loc)) || *(token_str + token_str_loc) == '_') {
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (isdigit(*(token_str + token_str_loc))) { // if next set of characters starts with a number
            while (isdigit(*(token_str + token_str_loc))) {
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == '\'') { // if next character is a single quote
            if (singleqt_str && squote_amnt != 1) { // if single quotes start and end a string
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                while (!(*(token_str + token_str_loc) == '\'' && num_esc % 2 == 0)) {
                    if (*(token_str + token_str_loc) == esc_char) {
                        num_esc++;
                    } else {
                        num_esc = 0;
                    }
                    token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (!even_sqt) {
                    squote_amnt -= 2;
                }
            } else if (singleqt_str && squote_amnt == 1 && ) {
                // DO NOW
            } else { // if single quotes don't start and end a string put as single token in token
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == '\"') { // if next character is a double quote
            if (doubleqt_str && dquote_amnt != 1) { // if double quotes start and end a string
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                while (!(*(token_str + token_str_loc) == '\"' && num_esc % 2 == 0)) {
                    if (*(token_str + token_str_loc) == esc_char) {
                        num_esc++;
                    } else {
                        num_esc = 0;
                    }
                    token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (!even_dqt) {
                    dquote_amnt -= 2;
                }
            } else if (doubleqt_str && dquote_amnt == 1) {
                // DO NOW
            } else { // if double quotes don't start and end a string put as single token in token
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (ispunct(*(token_str + token_str_loc))) { // if next character is a non-quote symbol
            token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
            *(token + token_loc) = *(token_str + token_str_loc);
            token_loc++;
            token_str_loc++;
        }

        if (strcmp(token, "")) { // if token is not empty
            *(token + token_loc) = '\0'; // end the current token with a null terminator
            *token_set = (char **) realloc(*token_set, sizeof(char *) * (current_token + 2)); // allocate one more set of memory in token_set to store token in.
            *(*token_set + current_token) = (char *) malloc(sizeof(char) * strlen(token)); // allocate the memory for the new token
            strcpy(*(*token_set + current_token), token); // copy token into token_set in the newly allocated section of memory
            token_loc = 0; // reset token_loc
            current_token++; // increase amount of tokens in token_set
        }
    }
    *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
    *(*token_set + current_token) = NULL;
    free(token);
}
