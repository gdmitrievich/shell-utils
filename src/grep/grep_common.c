#include "grep_common.h"

#include <stdlib.h>
#include <string.h>

bool add_pattern_to_patterns_string(char** patterns_str_ptr, const char* pattern) {
    bool status = true;
    if (*patterns_str_ptr && !append_str(patterns_str_ptr, "|")) status = false;
    if (!append_str(patterns_str_ptr, pattern)) status = false;
    return status;
}

char** add_str_to_str_arr_dynamically(char*** str_arr_ptr, const char* str) {
    if (!str) return *str_arr_ptr;

    bool status = true;
    char** p = NULL;
    size_t next_idx = 0;
    if (*str_arr_ptr) {
        size_t s = get_size_of_str_arr(*str_arr_ptr);
        p = (char**)realloc(*str_arr_ptr, sizeof(char**) * (s + 2));
        next_idx = s;
    } else {
        p = (char**)allocate_with_memset(sizeof(char**) * 2);
        next_idx = 0;
    }

    if (p) {
        *str_arr_ptr = p;
        if (!append_str(*str_arr_ptr + next_idx, str)) status = false;
        *(*str_arr_ptr + next_idx + 1) = NULL;
    } else {
        status = false;
    }

    return status ? *str_arr_ptr : NULL;
}

size_t get_size_of_str_arr(char** str_arr) {
    size_t s = 0;
    while (str_arr[s] != NULL) ++s;
    return s;
}

int has_new_line_char_at_the_end(const char* line) { return is_new_line_char(line[strlen(line) - 1]); }