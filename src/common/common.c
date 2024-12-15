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
}

void* allocate_with_memset(size_t size) {
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

int has_new_line_char_at_the_end(const char* line) { return is_new_line_char(line[strlen(line) - 1]); }
int is_new_line_char(char ch) { return ch == '\n'; }

void append_str(char** str, const char* src) {
    if (!src) return;

    char* ptr = NULL;
    if (*str)
        ptr = realloc(*str, strlen(*str) + strlen(src) + 1);
    else
        ptr = allocate_with_memset(strlen(src) + 1);
    if (ptr) {
        *str = ptr;
        strcat(*str, src);
    }
}

void substr(char* sub, const char* str, size_t start, size_t len) {
    memcpy(sub, &str[start], len);
    sub[len] = '\0';
}