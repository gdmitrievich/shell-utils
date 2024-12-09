#ifndef FLAG_PERFORMER_H
#define FLAG_PERFORMER_H

#include "flags.h"

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv);
void process_flags_on_line(flags flags, const char* line);

#endif // FLAG_PERFORMER_H
