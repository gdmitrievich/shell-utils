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