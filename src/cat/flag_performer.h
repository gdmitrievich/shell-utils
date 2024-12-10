#ifndef FLAG_PERFORMER_H
#define FLAG_PERFORMER_H

#include <stdio.h>

#include "flags.h"

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv);
int has_new_line_char_at_the_end(const char* line);
int is_new_line_char(char ch);
int fpeek(FILE* f);
FILE* read_new_line(char* line, int* i, int argc, char** argv);

void process_flags_on_line(flags flags, char* line);
void process_b_flag_on_line(const char* line);
int is_fully_empty_line(const char* line);

void process_E_flag_on_line(const char* line);
void print_chars_until_new_line_char(const char* line);
void process_n_flag_on_line(const char* line);
void process_s_flag_on_line(const char* line);
void process_T_flag_on_line(const char* line);
int is_tab(char ch);

void process_v_flag_on_line(char* line);
void strcat_formated_char_as_str(char* dest, const char* format, unsigned char ch);

#endif  // FLAG_PERFORMER_H
