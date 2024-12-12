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

void* try_reallocate_memory(const char* utility_name, void* src, size_t size) {
    void* new_ptr = realloc(src, size);
    if (!new_ptr) print_error(utility_name, NULL);

    return new_ptr;
}

int has_new_line_char_at_the_end(const char* line) { return is_new_line_char(line[strlen(line) - 1]); }
int is_new_line_char(char ch) { return ch == '\n'; }

void try_append_str(char** str, const char* src) {
    if (!src) return;

    if (*str)
        *str = try_reallocate_memory("grep", *str, strlen(*str) + strlen(src) + 1);
    else
        *str = try_allocate_memory("grep", strlen(src) + 1);
    strcat(*str, src);
}

void substr(char* sub, const char* str, size_t start, size_t len) {
    memcpy(sub, &str[start], len);
    sub[len] = '\0';
}