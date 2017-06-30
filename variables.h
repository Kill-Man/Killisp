#ifndef _VARIABLES_H_
#define _VARIABLES_H_

enum {
    INT,
    DOUBLE,
    CHAR_STRING
} datatypes;

typedef union variable {
    long long var_num;
    long double var_dec;
    char *var_str;
} variable;

// puts a datatype variable in key of value.
void put_var(const char *key, const char *value, const char *datatype);

//


#endif
