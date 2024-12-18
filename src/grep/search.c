#include "search.h"

#define BUFFSIZE 4096

#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include "grep_common.h"

bool set_matched_lines(matched_line** m_lines_ptr, cmd_args_data* cad) {
    bool status = true;
    if (cad->flags.f) status = set_regexes_retrieved_from_files(&(cad->patterns), cad->pattern_files);
    if (status) status = set_reg_exec_results_as_matched_lines(m_lines_ptr, cad);
    return status;
}

bool set_reg_exec_results_as_matched_lines(matched_line** m_lines_ptr, const cmd_args_data* cmd) {
    if (cmd->patterns == NULL) return false;

    bool status = true;
    regex_t regex;
    size_t error = 0;
    // REG_EXTENDED to use "|" in pattern.
    if ((error = regcomp(&regex, cmd->patterns, REG_EXTENDED | (cmd->flags.i ? REG_ICASE : 0))) != 0) {
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
            size_t line_num = 1;
            char* line = NULL;
            size_t line_len = 0;
            while (status && fgetdyns(&line, &line_len, fp)) {
                if (has_new_line_char_at_the_end(line)) line[strlen(line) - 1] = '\0';

                regmatch_t rm[1];
                int reg_state = regexec(&regex, line, 1, rm, 0);
                if ((reg_state == 0 && !cmd->flags.v) || (reg_state == REG_NOMATCH && cmd->flags.v)) {
                    if (cmd->flags.o) {
                        status = set_all_matches_from_line(m_lines_ptr, &regex, line, rm,
                                                           cmd->search_files[i], line_num);
                    } else {
                        int reg_state = regexec(&regex, line, 1, rm, 0);
                        if ((reg_state == 0 && !cmd->flags.v) || (reg_state == REG_NOMATCH && cmd->flags.v)) {
                            matched_line ml = {NULL, line_num, NULL};
                            if (!append_str(&ml.file_name, cmd->search_files[i])) status = false;
                            if (status && !append_str(&ml.line, line)) status = false;
                            if (status) status = append_matched_line(m_lines_ptr, &ml);
                        } else {
                            // Error.
                        }
                    }
                }

                ++line_num;
                // free(line);
                // line = NULL;
            }

            fclose(fp);
        } else if (!cmd->flags.s) {
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