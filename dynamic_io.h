#ifndef _DYNAMIC_IO_H_
#define _DYNAMIC_IO_H_

/// Stores an inputted string into a dynamically allocated char *.
/// string: pointer to the string that input is stored in. (use &)
/// error: String that is output if memory allocation for string fails.
/// error_code: Exit code if memory allocation for string fails.
void dynamic_gets(char **string, const char *error, const int error_code);

#endif // _DYNAMIC_IO_H_
