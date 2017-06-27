#include "strarr.h"
#include <stdlib.h>
#include <string.h>

void join_str(char **joined_str, char **str_arr, const char *conjoiner)
{
    free(*joined_str);
    int joined_str_len = str_arr_len(str_arr) + arr_count(str_arr) * (strlen(conjoiner) - 1);
    *joined_str = (char *) malloc(joined_str_len);
    strcpy(*joined_str, "");
    int i;
    for (i = 0; i < arr_count(str_arr) - 1; i++) {
        strcat(*joined_str, *(str_arr + i));
        strcat(*joined_str, conjoiner);
    }
    strcat(*joined_str, *(str_arr + i));
}

int str_arr_len(char **str_arr)
{
    int i, array_len = 0;
    for (i = 0; *(str_arr + i) != NULL; i++) {
        array_len += strlen(*(str_arr + i));
    }
    return array_len;
}

int arr_count(char **str_arr)
{
    int i;
    for (i = 0; *(str_arr + i) != NULL; i++);
    return i;
}

int str_starts_with(const char *pre, const char *str)
{
    int pre_len = strlen(pre);
    int str_len = strlen(str);
    return str_len < pre_len ? false : strncmp(pre, str, pre_len) == 0;
}

int count_amnt_str(const char *str, char **str_arr)
{
    int i, amnt = 0;
    for (i = 0; i < arr_count(str_arr); i++) {
        if (!strcmp(str, *(str_arr + i))) {
            amnt++;
        }
    }
    return amnt;
}

int str_in_arr(const char *str, char **str_arr)
{
    int i;
    for (i = 0; i < arr_count(str_arr); i++) {
        if (!strcmp(str, *(str_arr + i))) {
            return 1;
        }
    }
    return 0;
}
