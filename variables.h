#ifndef _VARIABLES_H_
#define _VARIABLES_H_

enum {
    INT,
    DOUBLE,
    CHAR_STRING
} datatypes;

typedef union variable {
    long long long_long;
    long double long_double;
    char *char_pointer;
} variable;

// puts a datatype variable in key of value.
void put_var(const char *key, const char *value, const char *datatype);

//


#endif
