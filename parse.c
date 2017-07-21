#include "parse.h"
#include "strarr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int  ind_blk, tab_val, dec_start, singleqt_str, doubleqt_str, esc_char_non_str, last_qt_str, whitespc, bgn_chars_in_wd;
static char esc_char;
static int  last_qt_unend;
static char *wd_chars, *bgn_chars, *begend_chars, *ender_chars;
static char closing_char;

// position variables
static int  token_loc; // int to know where in token the parser is
static int  current_token; // int to know where in token_set to store the value of *token
static int  token_str_loc; // int to know where in token_str the parser is
static char lack_str;
static int  missing_chars_loc;

// strings not static to be used outside file
char *missing_chars;

void parser_init(
    int   indent_block, /**/
    int   tab_value, /**/
    int   num_start_with_dec, //
    int   singlequote_str, //
    int   doublequote_str, //
    char  escape_character, //
    int   escape_character_non_string, //
    int   last_quote_str,
    int   whitespace, /**/
    char *word_chars, //
    char *begin_chars, //
    int   begin_chars_in_word, //
    char *chars_that_need_beginning_and_end,
    char *ending_chars
)
{
    ind_blk          = indent_block;
    tab_val          = tab_value;
    dec_start        = num_start_with_dec;
    singleqt_str     = singlequote_str;
    doubleqt_str     = doublequote_str;
    esc_char         = escape_character;
    esc_char_non_str = escape_character_non_string;
    last_qt_str      = last_quote_str;
    whitespc         = whitespace;
    wd_chars         = word_chars;
    bgn_chars        = begin_chars;
    bgn_chars_in_wd  = begin_chars_in_word;
    begend_chars     = chars_that_need_beginning_and_end;
    ender_chars      = ending_chars;
    if (last_qt_str) {
        last_qt_unend = 0;
    }
    missing_chars = (char *) malloc(sizeof(char)); // string to hold characters that need to be added to end the line.
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

int tokenize(char ***token_set, const char *token_str, const char *error, const int error_code)
{
    int i;
    int ret_val = 0;
    int missing_chars_loc = 0;
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
    free(missing_chars);
    closing_char = '\0';

    // allocating token_set
    *token_set = (char **) malloc (sizeof(char *));
    if (*token_set == NULL) {
        puts(error); // print error
        exit(error_code); // exit with error code
    }

    // allocating token
    char *token = (char *) malloc(sizeof(char)); // string to store current token
    if (token == NULL) {
        puts(error); // print error
        exit(error_code); // exit with error code
    }

    missing_chars = (char *) malloc(sizeof(char)); // string to hold characters that need to be added to end the line.
    if (missing_chars == NULL) {
        puts(error); // print error
        exit(error_code); // exit with error code
    }

    // position variables
    token_loc = 0; // int to know where in token the parser is
    current_token = 0; // int to know where in token_set to store the value of *token
    token_str_loc = 0; // int to know where in token_str the parser is

    // count non-escaped quotes in string
    char in_sqstring, in_dqstring; // chars to know if in string
    in_sqstring = in_dqstring = 0;
    int squote_amnt, dquote_amnt, squote_orig, dquote_orig;
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
    squote_orig = squote_amnt;
    dquote_orig = dquote_amnt;
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
        if (token == NULL) {
            puts(error); // print error
            exit(error_code); // exit with error code
        }

        // code for if whitespc is enabled
        if (whitespc) {
            // do later
        } else {
            while (isspace(*(token_str + token_str_loc))) {
                token_str_loc++;
            }
        }

        // if / else if / else for placing parsed string into token
        if (*(token_str + token_str_loc) != '\0' && (isalpha(*(token_str + token_str_loc)) || strchr(bgn_chars, *(token_str + token_str_loc)))) { // if next set of characters starts with a letter or number
            while (*(token_str + token_str_loc) != '\0' && (isalpha(*(token_str + token_str_loc)) || strchr(wd_chars, *(token_str + token_str_loc)) || (bgn_chars_in_wd && strchr(bgn_chars, *(token_str + token_str_loc))))) {
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (isdigit(*(token_str + token_str_loc)) || (dec_start && *(token_str + token_str_loc) == '.')) { // if next set of characters starts with a number
            int decimal_passed = 0;
            while (isdigit(*(token_str + token_str_loc)) || (*(token_str + token_str_loc) == '.' && !decimal_passed)) {
                if (*(token_str + token_str_loc) == '.') {
                    decimal_passed = 1;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == '\'') { // if next character is a single quote
            if (singleqt_str && squote_amnt != 1) { // if single quotes start and end a string
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

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
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (!even_sqt) {
                    squote_amnt -= 2;
                }
            } else if (singleqt_str && squote_amnt == 1 && last_qt_str) {
                // DO NOW
                ret_val = 1;
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                while (*(token_str + token_str_loc) != '\0') {
                    token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                *(token + token_loc) = '\0'; // end the current token with a null terminator
                *token_set = (char **) realloc(*token_set, sizeof(char *) * (current_token + 2)); // allocate one more set of memory in token_set to store token in.
                if (*token_set == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(*token_set + current_token) = (char *) malloc(sizeof(char) * strlen(token)); // allocate the memory for the new token
                if (*(*token_set + current_token) == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                strcpy(*(*token_set + current_token), token); // copy token into token_set in the newly allocated section of memory
                *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
                if (*(*token_set + current_token) == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(*token_set + current_token) = NULL;
                free(token);
                current_token++; // increase amount of tokens in token_set
                lack_str = '\'';
                return ret_val;
            } else { // if single quotes don't start and end a string put as single token in token
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == '\"') { // if next character is a double quote
            if (doubleqt_str && dquote_amnt != 1) { // if double quotes start and end a string
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

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
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (!even_dqt) {
                    dquote_amnt -= 2;
                }
            } else if (doubleqt_str && dquote_amnt == 1 && last_qt_str) {
                // DO NOW
                ret_val = 1;
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                while (*(token_str + token_str_loc) != '\0') {
                    token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                *(token + token_loc) = '\0'; // end the current token with a null terminator
                *token_set = (char **) realloc(*token_set, sizeof(char *) * (current_token + 2)); // allocate one more set of memory in token_set to store token in.
                if (*token_set == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(*token_set + current_token) = (char *) malloc(sizeof(char) * strlen(token)); // allocate the memory for the new token
                if (*(*token_set + current_token) == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                strcpy(*(*token_set + current_token), token); // copy token into token_set in the newly allocated section of memory
                *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
                if (*(*token_set + current_token) == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(*token_set + current_token) = NULL;
                free(token);
                current_token++; // increase amount of tokens in token_set
                lack_str = '\"';
                return ret_val;
            } else { // if double quotes don't start and end a string put as single token in token
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == esc_char && esc_char_non_str) { // next character is a backslash
            if (esc_char_non_str < 0) {
                token_str_loc += 2;
            } else if (esc_char_non_str > 0) {
                int a = 0;
                if (*(token_str + token_loc + 1) != '\0' && *(token_str + token_str_loc + 1) != EOF) {
                    a = 1;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2 + a));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (*(token_str + token_str_loc) != '\0' && *(token_str + token_str_loc) != EOF) {
                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
            }
        } else if (strchr(begend_chars, *(token_str + token_str_loc))) {
            char *ret  = strchr(begend_chars, *(token_str + token_str_loc));
            int index = (int) (ret - begend_chars);
            missing_chars = (char *) realloc(missing_chars, sizeof(char) * missing_chars_loc + 2);
            if (missing_chars == NULL) {
                puts(error);
                exit(error_code);
            }

            char temp_str[missing_chars_loc + 2];
            temp_str[0] = ender_chars[index];
            strcat(temp_str, missing_chars);
            strcpy(missing_chars, temp_str);
            missing_chars_loc++;
            closing_char = ender_chars[index];
        } else if (*(token_str + token_str_loc) == closing_char) {
            cpy_sub_str(missing_chars, missing_chars, 1, strlen(missing_chars));
            missing_chars_loc--;
            missing_chars = (char *) realloc(missing_chars, sizeof(char) * missing_chars_loc + 2);
            if (missing_chars == NULL) {
                puts(error);
                exit(error_code);
            }

            closing_char = missing_chars[0];
        } else if (ispunct(*(token_str + token_str_loc))) { // if next character is a non-quote symbol
            token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
            if (token == NULL) {
                puts(error); // print error
                exit(error_code); // exit with error code
            }

            *(token + token_loc) = *(token_str + token_str_loc);
            token_loc++;
            token_str_loc++;
        }

        if (strcmp(token, "")) { // if token is not empty
            *(token + token_loc) = '\0'; // end the current token with a null terminator
            *token_set = (char **) realloc(*token_set, sizeof(char *) * (current_token + 2)); // allocate one more set of memory in token_set to store token in.
            if (*token_set == NULL) {
                puts(error); // print error
                exit(error_code); // exit with error code
            }

            *(*token_set + current_token) = (char *) malloc(sizeof(char) * strlen(token)); // allocate the memory for the new token
            if (*(*token_set + current_token) == NULL) {
                puts(error); // print error
                exit(error_code); // exit with error code
            }

            strcpy(*(*token_set + current_token), token); // copy token into token_set in the newly allocated section of memory
            token_loc = 0; // reset token_loc
            current_token++; // increase amount of tokens in token_set
        }
    }
    *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
    if (*(*token_set + current_token) == NULL) {
        puts(error); // print error
        exit(error_code); // exit with error code
    }

    *(*token_set + current_token) = NULL;
    free(token);
    return ret_val;
}

int tokenize_append(char ***token_set, const char *token_str, const char *error, const int error_code)
{
    int i;
    int ret_val = 0;

    // allocating token
    char *token = (char *) malloc(sizeof(char)); // string to store current token
    if (token == NULL) {
        puts(error); // print error
        exit(error_code); // exit with error code
    }

    // position variables
    token_loc = 0; // int to know where in token the parser is
    current_token = 0; // int to know where in token_set to store the value of *token
    token_str_loc = 0; // int to know where in token_str the parser is

    // count non-escaped quotes in string
    char in_sqstring, in_dqstring; // chars to know if in string
    in_sqstring = in_dqstring = 0;
    if (last_qt_unend) {
        if (lack_str == '\'') {
            in_sqstring = 1;
        } else if (lack_str == '\"') {
            in_dqstring = 1;
        }
    }
    int squote_amnt, dquote_amnt, squote_orig, dquote_orig;
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
    squote_orig = squote_amnt;
    dquote_orig = dquote_amnt;
    num_esc = 0;
    char even_sqt, even_dqt;
    even_sqt = squote_amnt % 2 == 0;
    even_dqt = dquote_amnt % 2 == 0;

    if (last_qt_unend && squote_amnt == 0 && lack_str == '\'') {
        *(*token_set + current_token) = (char *) realloc(*(*token_set + current_token), sizeof(char) * (strlen(*(*token_set + current_token)) + strlen(token_str) + 1));
        if (*(*token_set + current_token) == NULL) {
            puts(error); // print error
            exit(error_code); // exit with error code
        }

        strcat(*(*token_set + current_token), "\n");
        strcat(*(*token_set + current_token), token_str);
        ret_val = 1;
        *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
        if (*(*token_set + current_token) == NULL) {
            puts(error); // print error
            exit(error_code); // exit with error code
        }

        *(*token_set + current_token) = NULL;
        free(token);
        return ret_val;
    } else if (last_qt_unend && dquote_amnt == 0 && lack_str == '\"') {
        *(*token_set + current_token) = (char *) realloc(*(*token_set + current_token), sizeof(char) * (strlen(*(*token_set + current_token)) + strlen(token_str) + 1));
        if (*(*token_set + current_token) == NULL) {
            puts(error); // print error
            exit(error_code); // exit with error code
        }

        strcat(*(*token_set + current_token), "\n");
        strcat(*(*token_set + current_token), token_str);
        ret_val = 1;
        *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
        if (*(*token_set + current_token) == NULL) {
            puts(error); // print error
            exit(error_code); // exit with error code
        }

        *(*token_set + current_token) = NULL;
        free(token);
        return ret_val;
    }

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
        if (token == NULL) {
            puts(error); // print error
            exit(error_code); // exit with error code
        }

        // code for if whitespc is enabled
        if (whitespc) {
            // do later
        } else {
            while (isspace(*(token_str + token_str_loc))) {
                token_str_loc++;
            }
        }

        // if / else if / else for placing parsed string into token
        if (*(token_str + token_str_loc) != '\0' && (isalpha(*(token_str + token_str_loc)) || strchr(bgn_chars, *(token_str + token_str_loc)))) { // if next set of characters starts with a letter or number
            while (*(token_str + token_str_loc) != '\0' && (isalpha(*(token_str + token_str_loc)) || strchr(wd_chars, *(token_str + token_str_loc)) || (bgn_chars_in_wd && strchr(bgn_chars, *(token_str + token_str_loc))))) {
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (isdigit(*(token_str + token_str_loc)) || (dec_start && *(token_str + token_str_loc) == '.')) { // if next set of characters starts with a number
            int decimal_passed = 0;
            while (isdigit(*(token_str + token_str_loc)) || (*(token_str + token_str_loc) == '.' && !decimal_passed)) {
                if (*(token_str + token_str_loc) == '.') {
                    decimal_passed = 1;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == '\'') { // if next character is a single quote
            if (singleqt_str && squote_amnt != 1) { // if single quotes start and end a string
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

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
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (!even_sqt) {
                    squote_amnt -= 2;
                }
            } else if (singleqt_str && squote_amnt == 1 && last_qt_str) {
                // DO NOW
                ret_val = 1;
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                while (*(token_str + token_str_loc) != '\0') {
                    token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            } else { // if single quotes don't start and end a string put as single token in token
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == '\"') { // if next character is a double quote
            if (doubleqt_str && dquote_amnt != 1) { // if double quotes start and end a string
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

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
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (!even_dqt) {
                    dquote_amnt -= 2;
                }
            } else if (doubleqt_str && dquote_amnt == 1 && last_qt_str) {
                // DO NOW
                ret_val = 1;
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                while (*(token_str + token_str_loc) != '\0') {
                    token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                    if (token == NULL) {
                        puts(error); // print error
                        exit(error_code); // exit with error code
                    }

                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            } else { // if double quotes don't start and end a string put as single token in token
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
            }
        } else if (*(token_str + token_str_loc) == esc_char && esc_char_non_str) { // next character is a backslash
            if (esc_char_non_str < 0) {
                token_str_loc += 2;
            } else if (esc_char_non_str > 0) {
                int a = 0;
                if (*(token_str + token_loc + 1) != '\0' && *(token_str + token_str_loc + 1) != EOF) {
                    a = 1;
                }
                token = (char *) realloc(token, sizeof(char) * (token_loc + 2 + a));
                if (token == NULL) {
                    puts(error); // print error
                    exit(error_code); // exit with error code
                }

                *(token + token_loc) = *(token_str + token_str_loc);
                token_loc++;
                token_str_loc++;
                if (*(token_str + token_str_loc) != '\0' && *(token_str + token_str_loc) != EOF) {
                    *(token + token_loc) = *(token_str + token_str_loc);
                    token_loc++;
                    token_str_loc++;
                }
            }
        } else if (strchr(begend_chars, *(token_str + token_str_loc))) {
            char *ret  = strchr(begend_chars, *(token_str + token_str_loc));
            int index = (int) (ret - begend_chars);
            missing_chars = (char *) realloc(missing_chars, sizeof(char) * missing_chars_loc + 2);
            if (missing_chars == NULL) {
                puts(error);
                exit(error_code);
            }

            char temp_str[missing_chars_loc + 2];
            temp_str[0] = ender_chars[index];
            strcat(temp_str, missing_chars);
            strcpy(missing_chars, temp_str);
            missing_chars_loc++;
            closing_char = ender_chars[index];
        } else if (*(token_str + token_str_loc) == closing_char) {
            cpy_sub_str(missing_chars, missing_chars, 1, strlen(missing_chars));
            missing_chars_loc--;
            missing_chars = (char *) realloc(missing_chars, sizeof(char) * missing_chars_loc + 2);
            if (missing_chars == NULL) {
                puts(error);
                exit(error_code);
            }

            closing_char = missing_chars[0];
        } else if (ispunct(*(token_str + token_str_loc))) { // if next character is a non-quote symbol
            token = (char *) realloc(token, sizeof(char) * (token_loc + 2));
            if (token == NULL) {
                puts(error); // print error
                exit(error_code); // exit with error code
            }

            *(token + token_loc) = *(token_str + token_str_loc);
            token_loc++;
            token_str_loc++;
        }

        if (strcmp(token, "")) { // if token is not empty
            *(token + token_loc) = '\0'; // end the current token with a null terminator
            *token_set = (char **) realloc(*token_set, sizeof(char *) * (current_token + 2)); // allocate one more set of memory in token_set to store token in.
            if (*token_set == NULL) {
                puts(error); // print error
                exit(error_code); // exit with error code
            }

            *(*token_set + current_token) = (char *) malloc(sizeof(char) * strlen(token)); // allocate the memory for the new token
            if (*(*token_set + current_token) == NULL) {
                puts(error); // print error
                exit(error_code); // exit with error code
            }

            strcpy(*(*token_set + current_token), token); // copy token into token_set in the newly allocated section of memory
            token_loc = 0; // reset token_loc
            current_token++; // increase amount of tokens in token_set
        }
    }
    *(*token_set + current_token) = (char *) malloc(sizeof(NULL));
    if (*(*token_set + current_token) == NULL) {
        puts(error); // print error
        exit(error_code); // exit with error code
    }

    *(*token_set + current_token) = NULL;
    free(token);
    return ret_val;
}
