#ifndef FLAG_PERFORMER_H
#define FLAG_PERFORMER_H

#include "flags.h"

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv);
void process_flags_on_line(flags flags, const char* line);
void process_b_flag_on_line(const char* line);
int is_fully_empty_line(const char* line);

#endif // FLAG_PERFORMER_H
