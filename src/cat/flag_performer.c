#include "flag_performer.h"

#include <stdio.h>
#include <stdlib.h>

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv) {
    char line[256] = {0};
    for (int i = first_filepath_idx; i < argc; ++i) {
	   	char* state = NULL;
		FILE* f = fopen(argv[i], "r");

        while ((state = fgets(line, sizeof(line), f)))
    		process_flags_on_line(flags, line);

        fclose(f);
    }
}