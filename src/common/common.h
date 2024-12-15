#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

typedef enum { false, true } bool;

void print_error(const char* utility_name, const char* message);

void* allocate_with_memset(size_t size);

int has_new_line_char_at_the_end(const char* line);
int is_new_line_char(char ch);

void append_str(char** str, const char* src);
void substr(char* sub, const char* str, size_t start, size_t len);

#endif  // COMMON_H