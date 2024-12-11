#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

void print_error(const char* utility_name, const char* message);
void print_error_if_cant_open_file(const char* utility_name, const char* filename, const FILE* f);

void* try_allocate_memory(const char* utility_name, size_t size);
void* try_reallocate_memory(const char* utility_name, void* src, size_t new_size);

int has_new_line_char_at_the_end(const char* line);
int is_new_line_char(char ch);

void try_append_str(char** str, const char* src);

#endif // COMMON_H