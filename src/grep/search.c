#include "search.h"

#define BUFFSIZE 4096

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "grep_common.h"

bool set_matched_lines_with_patterns_from_file(matched_line** m_lines_ptr, const cmd_args_data* cad,
                                               const char* search_file) {
    if (cad->patterns == NULL) return false;

    bool status = true;
    regex_t regex;
    size_t error = 0;
    // REG_EXTENDED to use "|" in pattern.
    if ((error = regcomp(&regex, cad->patterns, REG_EXTENDED | (cad->flags.i ? REG_ICASE : 0))) != 0) {
        // If errbuf_size is 0 returns the size of the buffer needed to hold the generated string.
        size_t err_len = regerror(error, &regex, (char*)NULL, 0);
        char* err_buf = (char*)allocate_with_memset(err_len);
        if (err_buf) {
            regerror(error, &regex, err_buf, err_len);
            print_error("grep", err_buf);
            status = false;
            free(err_buf);
        }
    }

    FILE* fp = fopen(search_file, "r");
    if (fp) {
        size_t line_num = 1;
        char* line = NULL;
        size_t line_len = 0;
        while (status && fgetdyns(&line, &line_len, fp)) {
            if (has_new_line_char_at_the_end(line)) line[strlen(line) - 1] = '\0';

            regmatch_t rm[1];
            int reg_state = regexec(&regex, line, 1, rm, 0);
            if ((reg_state == 0 && !cad->flags.v) || (reg_state == REG_NOMATCH && cad->flags.v)) {
                if (cad->flags.o) {
                    status = set_all_matches_from_line(m_lines_ptr, &regex, line, rm, search_file, line_num);
                } else {
                    int reg_state = regexec(&regex, line, 1, rm, 0);
                    if ((reg_state == 0 && !cad->flags.v) || (reg_state == REG_NOMATCH && cad->flags.v)) {
                        matched_line ml = {NULL, line_num, NULL};
                        if (!append_str(&ml.file_name, search_file)) status = false;
                        if (status && !append_str(&ml.line, line)) status = false;
                        if (status) status = append_matched_line(m_lines_ptr, &ml);
                    } else {
                        // Error.
                    }
                }
            }

            ++line_num;
            free(line);
            line = NULL;
        }

        fclose(fp);
    } else if (!cad->flags.s) {
        print_error("grep", search_file);
    }

    regfree(&regex);
    return status;
}

bool set_all_matches_from_line(matched_line** m_lines, regex_t* regex, char* str, regmatch_t* rm,
                               const char* file_name, size_t line_number) {
    bool status = true;
    bool reg_state = 0;
    while (status && reg_state == 0) {
        reg_state = regexec(regex, str, 1, rm, 0);
        if (reg_state == 0 && *str) {
            matched_line ml = {NULL, line_number, NULL};
            if (!append_str(&ml.file_name, file_name)) status = false;

            int len = rm[0].rm_eo - rm[0].rm_so;
            char line[len + 1];
            substr(line, str, rm[0].rm_so, len);
            if (status && !append_str(&ml.line, line)) status = false;

            if (status) status = append_matched_line(m_lines, &ml);
            if (status) str += rm[0].rm_eo;
        }
    }
    return status;
}