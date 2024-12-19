#include "common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(const char* utility_name, const char* message) {
    if (!utility_name && !message) return;

    if (!message) {
        perror(utility_name);
    } else if (errno) {
        fprintf(stderr, "%s: %s: %s\n", utility_name, message, strerror(errno));
    } else {
        fprintf(stderr, "%s: %s\n", utility_name, message);
    }

    errno = 0;
}

void* allocate_with_memset(size_t size) {
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

int is_new_line_char(char ch) { return ch == '\n'; }

char** append_str(char** str, const char* src) {
    if (!src) return str;

    char* ptr = NULL;
    bool status = false;
    if (*str)
        ptr = realloc(*str, strlen(*str) + strlen(src) + 1);
    else
        ptr = allocate_with_memset(strlen(src) + 1);
    if (ptr) {
        *str = ptr;
        strcat(*str, src);
        status = true;
    }
    return status ? str : NULL;
}

char** append_binary_str(char** str, size_t str_len, const char* src, size_t src_len) {
    if (!src) return str;

    char* ptr = NULL;
    bool status = false;
    if (*str)
        ptr = realloc(*str, str_len + src_len + 1);
    else
        ptr = allocate_with_memset(src_len + 1);
    if (ptr) {
        *str = ptr;
        memcpy(*str + str_len, src, src_len);
        status = true;
    }
    return status ? str : NULL;
}

void substr(char* sub, const char* str, size_t start, size_t len) {
    memcpy(sub, &str[start], len);
    sub[len] = '\0';
}

char* fgetdyns(char** str_ptr, size_t* n, FILE* stream) {
    *str_ptr = NULL;
    *n = 0;
    size_t capacity = 4;

    int ch = 0;
    bool state = true;
    bool new_line_found = false;
    if ((ch = fgetc(stream)) != EOF) {
        ungetc(ch, stream);
        *str_ptr = allocate_with_memset(capacity);
        if (str_ptr) {
            while (state && !new_line_found && (ch = fgetc(stream)) != EOF) {
                state = add_char_to_str(ch, str_ptr, (*n)++, &capacity);
                if (ch == '\n') new_line_found = true;
            }
            if (state) {
                state = add_char_to_str('\0', str_ptr, *n, &capacity);
            }
            if (state && ch == EOF) {
                ungetc(ch, stream);
                ch = 0;
            }
        } else {
            state = false;
        }
    }

    return state ? ch != EOF ? *str_ptr : NULL : NULL;
}

bool add_char_to_str(int ch, char** str_ptr, size_t pos, size_t* capacity) {
    bool state = true;
    if (pos + 1 >= *capacity) {
        *capacity *= 2;
        char* p = (char*)realloc(*str_ptr, *capacity);
        if (p) {
            *str_ptr = p;
        } else {
            state = false;
        }
    }
    if (state) (*str_ptr)[pos] = ch;
    return state;
}