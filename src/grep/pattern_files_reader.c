#include "pattern_files_reader.h"

#include "grep_common.h"
#include <string.h>
#include <stdlib.h>

bool set_regexes_retrieved_from_files(char** patterns_ptr, char** pattern_files) {
    bool status = true;
    for (size_t i = 0; status && pattern_files[i] != NULL; ++i) {
        FILE* fp = fopen(pattern_files[i], "r");
        if (fp) {
            set_regexes_retrieved_from_file(patterns_ptr, fp);
            fclose(fp);
        } else {
            print_error("grep", pattern_files[i]);
        }
    }

    return status;
}

bool set_regexes_retrieved_from_file(char** patterns_ptr, FILE* fp) {
    char* line = NULL;
    size_t line_len = 0;
    bool status = true;
    while (status && fgetdyns(&line, &line_len, fp) != NULL) {
        if (has_new_line_char_at_the_end(line)) line[strlen(line) - 1] = '\0';

        if (*line == '\0') {
            if (!add_pattern_to_patterns_string(patterns_ptr, ".")) status = false;
        } else {
            if (!add_pattern_to_patterns_string(patterns_ptr, line)) status = false;
        }
        free(line);
        line = NULL;
    }

    return status;
}