#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(const char* utility_name, const char* message) {
    if (!utility_name && !message) return;

    if (!message)
        perror(utility_name);
    else if (errno)
        fprintf(stderr, "%s: %s: %s\n", utility_name, message, strerror(errno));
    else
        fprintf(stderr, "%s: %s\n", utility_name, message);
    exit(EXIT_FAILURE);
}

void print_error_if_cant_open_file(const char* utility_name, const char* filename, const FILE* f) {
    if (!f) print_error(utility_name, filename);
}

void* try_allocate_memory(const char* utility_name, size_t size) {
    void* ptr = malloc(size);
    if (!ptr) print_error(utility_name, NULL);

    memset(ptr, 0, size);
    return ptr;
}

void* try_reallocate_memory(const char* utility_name, void* src, size_t init_size, size_t new_size) {
    void* new_ptr = try_allocate_memory(utility_name, new_size);

    memcpy(new_ptr, src, init_size);
    memset(new_ptr + init_size, 0, new_size - init_size);
	free(src);

    return new_ptr;
}

int has_new_line_char_at_the_end(const char* line) { return is_new_line_char(line[strlen(line) - 1]); }
int is_new_line_char(char ch) { return ch == '\n'; }

void try_append_str(char** str, const char* src) {
    if (!src) return;

    if (*str)
        *str = try_reallocate_memory("grep", *str, strlen(*str) + 1, strlen(*str) + strlen(src) + 1);
    else
        *str = try_allocate_memory("grep", strlen(src) + 1);
    strcat(*str, src);
}