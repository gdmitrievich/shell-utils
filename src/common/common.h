#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

typedef enum { false, true } bool;

void print_error(const char* utility_name, const char* message);

void* allocate_with_memset(size_t size);

int has_new_line_char_at_the_end(const char* line);
int is_new_line_char(char ch);

/// @brief Strcat src to str. If str is NULL, then new memory for str will be allocated.
/// @return str if src is NULL or memory for extended str successfully allocated, otherwise NULL.
char** append_str(char** str, const char* src);
void substr(char* sub, const char* str, size_t start, size_t len);

/// @brief Reads in at most one less than n characters from stream and stores them into the dynamically
/// allocated str_ptr. Reading stops after an EOF or a newline. If a newline is read, it is stored into the
/// str_ptr. A terminating null byte ('\0') is stored after the last character in the str_ptr. If
/// @param n Stores the number of read characters except last terminating null byte ('\0').
/// @return Pointer to allocated string pointed by str_ptr on success, and NULL on error or when end of file
/// occurs while no characters have been read.
char* fgetdyns(char** str_ptr, size_t* n, FILE* stream);
bool add_char_to_str(int ch, char** str_ptr, size_t pos, size_t* capacity);

#endif  // COMMON_H