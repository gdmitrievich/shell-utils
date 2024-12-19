#include "search.h"

#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "grep_common.h"

bool set_matched_lines_with_patterns_from_file(matched_line** m_lines_ptr, const cmd_args_data* cad,
                                               const char* search_file, bool* file_found) {
    if (cad->patterns == NULL) return false;
    bool status = true;
    regex_t regex;
    size_t error = 0;
    if ((error = regcomp(&regex, cad->patterns, REG_EXTENDED | (cad->flags.i ? REG_ICASE : 0))) >
        REG_NOMATCH) {
        output_regex_error(error, &regex);
        status = false;
    }
    FILE* fp = fopen(search_file, "r");
    if (status && fp) {
        size_t line_num = 1;
        char* line = NULL;
        size_t line_len = 0;
        while (status && fgetdyns(&line, &line_len, fp)) {
            if (has_new_line_char_at_the_end(line)) line[strlen(line) - 1] = '\0';
            regexec_on_line(&regex, line, cad, m_lines_ptr, search_file, line_num);
            ++line_num;
            free(line);
            line = NULL;
        }
        fclose(fp);
        *file_found = true;
    } else if (status && !cad->flags.s) {
        print_error("grep", search_file);
        *file_found = false;
    } else if (!status) {
        errno = 0;
    }
    regfree(&regex);
    return status;
}

void output_regex_error(size_t error, regex_t* regex_ptr) {
    // If errbuf_size is 0 returns the size of the buffer needed to hold the generated string.
    size_t err_len = regerror(error, regex_ptr, (char*)NULL, 0);
    char* err_buf = (char*)allocate_with_memset(err_len);
    if (err_buf) {
        regerror(error, regex_ptr, err_buf, err_len);
        print_error("grep", err_buf);
        free(err_buf);
    }
}

bool regexec_on_line(regex_t* regex_ptr, const char* line, const cmd_args_data* cad,
                     matched_line** m_lines_ptr, const char* search_file, size_t line_num) {
    bool status = true;
    regmatch_t rm[1];
    int error = regexec(regex_ptr, line, 1, rm, 0);
    if ((error == 0 && !cad->flags.v) || (error == REG_NOMATCH && cad->flags.v)) {
        if (cad->flags.o) {
            status = set_all_matches_from_line(m_lines_ptr, regex_ptr, line, rm, search_file, line_num);
        } else {
            matched_line ml = {NULL, 0, NULL};
            status = fill_matched_line(&ml, line_num, search_file, line);
            if (status) status = append_matched_line(m_lines_ptr, &ml);
            free_matched_line(&ml);
        }
    } else if (error > REG_NOMATCH) {
        output_regex_error(error, regex_ptr);
        status = false;
    }
    return status;
}

bool set_all_matches_from_line(matched_line** m_lines, regex_t* regex, const char* str, regmatch_t* rm,
                               const char* file_name, size_t line_number) {
    bool status = true;
    bool reg_state = 0;
    while (status && reg_state == 0) {
        reg_state = regexec(regex, str, 1, rm, 0);
        if (reg_state == 0 && *str) {
            matched_line ml = {NULL, 0, NULL};
            int len = rm[0].rm_eo - rm[0].rm_so;
            char line[len + 1];
            substr(line, str, rm[0].rm_so, len);
            status = fill_matched_line(&ml, line_number, file_name, line);
            if (status) status = append_matched_line(m_lines, &ml);
            if (status) {
                str += rm[0].rm_eo;
            }
            free_matched_line(&ml);
        }
    }
    return status;
}
