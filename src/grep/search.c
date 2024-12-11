#include "search.h"

#define BUFFSIZE 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void set_regex_matches(regex_matches* rm, cmd_args_data* cad) {
    set_retrieved_regexes_from_file(&(cad->pattern), cad->pattern_file);
    set_reg_exec_results(cad->pattern, cad->flags, rm);
}

void set_retrieved_regexes_from_file(char** pattern_ptr, const char* pattern_file) {
    FILE* fp = fopen(pattern_file, "r");
    print_error_if_cant_open_file("grep", pattern_file, fp);

    char buffer[BUFFSIZE] = {0};
    while (fgets(buffer, BUFFSIZE, fp) != NULL) {
        if (has_new_line_char_at_the_end(buffer)) buffer[strlen(buffer) - 1] = '\0';

        if (*pattern_ptr) try_append_str(pattern_ptr, "|");

        if (*buffer == '\0') {
            try_append_str(pattern_ptr, ".");
        } else {
            try_append_str(pattern_ptr, buffer);
        }
    }

    fclose(fp);
}

void set_reg_exec_results(char* pattern, flags flags, regex_matches* regexms) {}