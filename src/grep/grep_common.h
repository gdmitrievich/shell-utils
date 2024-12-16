#ifndef GREP_COMMON_H
#define GREP_COMMON_H

#include "../common/common.h"

bool add_pattern_to_patterns_string(char** patterns_str_ptr, const char* pattern);
char** add_str_to_str_arr_dynamically(char*** str_arr_ptr, const char* str);
size_t get_size_of_str_arr(char** str_arr);

#endif  // GREP_COMMON_H