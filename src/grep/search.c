#include "search.h"

#define BUFFSIZE 4096

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../common/common.h"

bool set_matched_lines(matched_line** m_lines_ptr, cmd_args_data* cad) {
    bool status = true;
    if (cad->flags.f) status = set_retrieved_regexes_from_file(&(cad->pattern), cad->pattern_file);
    if (status) status = set_reg_exec_results_as_matched_lines(m_lines_ptr, cad);
    return status;
}

bool set_retrieved_regexes_from_file(char** pattern_ptr, const char* pattern_file) {
    FILE* fp = fopen(pattern_file, "r");
    bool status = true;
    if (fp) {
        char buffer[BUFFSIZE] = {0};
        while (status && fgets(buffer, BUFFSIZE, fp) != NULL) {
            if (has_new_line_char_at_the_end(buffer)) buffer[strlen(buffer) - 1] = '\0';

            if (*pattern_ptr) status = append_str(pattern_ptr, "|");

            if (status && *buffer == '\0') {
                status = append_str(pattern_ptr, ".");
            } else if (status) {
                status = append_str(pattern_ptr, buffer);
            }
        }

        fclose(fp);
    } else {
        print_error("grep", pattern_file);
        status = false;
    }

    return status;
}

bool set_reg_exec_results_as_matched_lines(matched_line** m_lines_ptr, const cmd_args_data* cmd) {
    if (cmd->pattern == NULL) return false;

    bool status = true;
    regex_t regex;
    size_t error = 0;
    // REG_EXTENDED to use "|" in pattern.
    if ((error = regcomp(&regex, cmd->pattern, REG_EXTENDED | (cmd->flags.i ? REG_ICASE : 0))) != 0) {
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

    for (int i = 0; status && cmd->search_files[i] != NULL; ++i) {
        FILE* fp = fopen(cmd->search_files[i], "r");
        if (fp) {
            char buf[BUFFSIZE] = {0};
            size_t line_num = 1;
            while (status && fgets(buf, BUFFSIZE, fp)) {
                if (has_new_line_char_at_the_end(buf)) buf[strlen(buf) - 1] = '\0';

                regmatch_t rm[1];
                int reg_state = regexec(&regex, buf, 1, rm, 0);
                if ((reg_state == 0 && !cmd->flags.v) || (reg_state == REG_NOMATCH && cmd->flags.v)) {
                    if (cmd->flags.o) {
                        status = set_all_matches_from_line(m_lines_ptr, &regex, buf, rm, cmd->search_files[i],
                                                           line_num);
                    } else {
                        int reg_state = regexec(&regex, buf, 1, rm, 0);
                        if ((reg_state == 0 && !cmd->flags.v) || (reg_state == REG_NOMATCH && cmd->flags.v)) {
                            matched_line ml = {NULL, line_num, NULL};
                            status = append_str(&ml.file_name, cmd->search_files[i]);
                            if (status) status = append_str(&ml.line, buf);
                            if (status) status = append_matched_line(m_lines_ptr, &ml);
                        } else {
                            // Error.
                        }
                    }
                }

                ++line_num;
            }

            fclose(fp);
        } else {
            print_error("grep", cmd->search_files[i]);
        }
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
            status = append_str(&ml.file_name, file_name);

            int len = rm[0].rm_eo - rm[0].rm_so;
            char line[len + 1];
            substr(line, str, rm[0].rm_so, len);
            if (status) status = append_str(&ml.line, line);

            if (status) status = append_matched_line(m_lines, &ml);
            if (status) str += rm[0].rm_eo;
        }
    }
    return status;
}