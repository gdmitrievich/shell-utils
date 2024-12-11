#include "search.h"

#define BUFFSIZE 4096

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../common/common.h"

void set_regex_matches(regex_matches* rm, cmd_args_data* cad) {
    set_retrieved_regexes_from_file(&(cad->pattern), cad->pattern_file);
    set_reg_exec_results(cad, rm);
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

void set_reg_exec_results(const cmd_args_data* cmd, regex_matches* regexms) {
    if (cmd->pattern == NULL) return;

    regex_t regex;
    size_t error = 0;
    if ((error = regcomp(&regex, cmd->pattern, REG_EXTENDED)) != 0) {  // REG_EXTENDED to use "|" in pattern.
        // If errbuf_size is 0 returns the size of the buffer needed to hold the generated string.
        size_t err_len = regerror(error, &regex, (char*)NULL, 0);

        char* err_buf = (char*)try_allocate_memory("grep", err_len);
        regerror(error, &regex, err_buf, err_len);
        print_error("grep", err_buf);
        free(err_buf);
    }

    for (int i = 0; cmd->search_files[i] != NULL; ++i) {
        matched_file matched_file;
        init_mf(&matched_file);

        FILE* fp = fopen(cmd->search_files[i], "r");
        print_error_if_cant_open_file("grep", cmd->search_files[i], fp);

        try_append_str(&matched_file.name, cmd->search_files[i]);
        char buf[BUFFSIZE] = {0};
        size_t nline = 1;
        while (fgets(buf, BUFFSIZE, fp)) {
            int err = 0;
            const int NMATCH = 512;
            regmatch_t rt[NMATCH];
            if ((err = regexec(&regex, buf, NMATCH, rt, 0)) == 0) {
                matched_line ml;
                init_ml(&ml);
                ml.number = nline;
                try_append_str(&ml.content, buf);
                try_append_to_matched_lines(&matched_file.matched_lines, &ml);
            } else if (err == REG_NOMATCH) {
            } else {
                // Error.
            }
            ++nline;
        }

        try_append_to_matched_files(&regexms->matched_files, &matched_file);
        fclose(fp);
    }

    regfree(&regex);
}