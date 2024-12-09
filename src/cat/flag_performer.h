#ifndef FLAG_PERFORMER_H
#define FLAG_PERFORMER_H

#include "flags.h"
#include <stdio.h>

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv);
int has_new_line_char_at_the_end(const char* line);
int fpeek(FILE* f);
FILE* read_new_line(char* line, int* i, int argc, char** argv);

void process_flags_on_line(flags flags, const char* line);
void process_b_flag_on_line(const char* line);
int is_fully_empty_line(const char* line);

#endif // FLAG_PERFORMER_H
