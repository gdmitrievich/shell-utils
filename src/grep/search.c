#include "search.h"

#define BUFFSIZE 4096

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../common/common.h"

matched_line* get_matched_lines(cmd_args_data* cad) {
    if (cad->flags.f) set_retrieved_regexes_from_file(&(cad->pattern), cad->pattern_file);
    return get_reg_exec_results_as_matched_lines(cad);
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

matched_line* get_reg_exec_results_as_matched_lines(const cmd_args_data* cmd) {
    if (cmd->pattern == NULL) return NULL;

    matched_line* matched_lines = NULL;
    regex_t regex;
    size_t error = 0;
    // REG_EXTENDED to use "|" in pattern.
    if ((error = regcomp(&regex, cmd->pattern, REG_EXTENDED | (cmd->flags.i ? REG_ICASE : 0))) != 0) {
        // If errbuf_size is 0 returns the size of the buffer needed to hold the generated string.
        size_t err_len = regerror(error, &regex, (char*)NULL, 0);

        char* err_buf = (char*)try_allocate_memory("grep", err_len);
        regerror(error, &regex, err_buf, err_len);
        print_error("grep", err_buf);
        free(err_buf);
    }

    for (int i = 0; cmd->search_files[i] != NULL; ++i) {
        FILE* fp = fopen(cmd->search_files[i], "r");
        print_error_if_cant_open_file("grep", cmd->search_files[i], fp);

        char buf[BUFFSIZE] = {0};
        size_t line_num = 1;
        while (fgets(buf, BUFFSIZE, fp)) {
            regmatch_t rm[1];
            int state = regexec(&regex, buf, 1, rm, 0);
            if ((state == 0 && !cmd->flags.v) || (state == REG_NOMATCH && cmd->flags.v)) {
                if (cmd->flags.o) {
                    set_all_matches_from_line(&matched_lines, &regex, buf, rm, cmd->search_files[i],
                                              line_num);
                } else {
                    int state = regexec(&regex, buf, 1, rm, 0);
                    if (state == 0) {
                        matched_line ml = {NULL, line_num, NULL};
                        try_append_str(&ml.file_name, cmd->search_files[i]);

                        if (has_new_line_char_at_the_end(buf)) buf[strlen(buf) - 1] = '\0';
                        try_append_str(&ml.line, buf);

                        try_append_matched_line(&matched_lines, &ml);
                    } else {
                        // Error.
                    }
                }
            }

            ++line_num;
        }

        fclose(fp);
    }

    regfree(&regex);
    return matched_lines;
}

void set_all_matches_from_line(matched_line** m_lines, regex_t* regex, char* str, regmatch_t* rm,
                               const char* file_name, size_t line_number) {
    int state = 0;
    while (state == 0) {
        state = regexec(regex, str, 1, rm, REG_NOTBOL);
        if (state == 0 && *str) {
            matched_line ml = {NULL, line_number, NULL};
            try_append_str(&ml.file_name, file_name);

            int len = rm[0].rm_eo - rm[0].rm_so;
            char line[len + 1];
            substr(line, str, rm[0].rm_so, len);
            try_append_str(&ml.line, line);

            try_append_matched_line(m_lines, &ml);

            str += rm[0].rm_eo;
        }
    }
}