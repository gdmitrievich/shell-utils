#include "flag_performer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv) {
    char line[256] = {0};
    for (int i = first_filepath_idx; i < argc; ++i) {
        char* state = NULL;
        FILE* f = fopen(argv[i], "rb");

        while ((state = fgets(line, sizeof(line), f))) process_flags_on_line(flags, line);

        fclose(f);
    }
}

void process_flags_on_line(flags flags, const char* line) {
    if (flags.b) process_b_flag_on_line(line);
    //if (flags.E) process_E_flag_on_line(line);
    //if (flags.n) process_n_flag_on_line(line);
    //if (flags.s) process_s_flag_on_line(line);
    //if (flags.T) process_T_flag_on_line(line);
    //if (flags.v) process_v_flag_on_line(line);
}

void process_b_flag_on_line(const char* line) {
	static int nLine = 1;
    if (!is_fully_empty_line(line)) {
		printf("%6d\t%s", nLine++, line);
    } else {
        printf("%s", line);
    }
}

int is_fully_empty_line(const char* line) {
	return line[0] == '\n';
}