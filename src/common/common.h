#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

void print_error(const char* utility_name, const char* message);
void print_error_if_cant_open_file(const char* utility_name, const char* filename, const FILE* f);

void* try_allocate_memory(const char* utility_name, size_t size);

#endif // COMMON_H