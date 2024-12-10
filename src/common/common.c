#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void print_error(const char* utility_name, const char* message) {
	if (!utility_name && !message) return;

	if (!message)
		perror(utility_name);
	else
		fprintf(stderr, "%s: %s: %s", utility_name, message, strerror(errno));
	exit(EXIT_FAILURE);
}

void print_error_if_cant_open_file(const char* utility_name, const char* filename, FILE* f) {
	if (!f)
		print_error(utility_name, filename);
}

void* try_allocate_memory(const char* utility_name, size_t size) {
	void* ptr = malloc(size);
	if (!ptr)
		print_error(utility_name, NULL);

	memset(ptr, 0, size);
	return ptr;
}