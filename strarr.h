#ifndef _STRARR_H_
#define _STRARR_H_

// joins strings in char **str_arr using conjoiner into joined_str
void join_str(char **joined_str, char **str_arr, const char *conjoiner);

// gets length of all strings in str_arr combined
int str_arr_len(char **str_arr);

// counts strings in str_arr
int arr_count(char **str_arr);

// checks of str starts with pre
int str_starts_with(const char *pre, const char *str);

// counts the amount of times str is in str_arr
int count_amnt_str(const char *str, char **str_arr);

// checks if str appears in str_arr
int str_in_arr(const char *str, char **str_arr);

#endif // _STRARR_H_
