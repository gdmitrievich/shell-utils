#include "output.h"

#include <stdio.h>

void output(const matched_line* m_lines, const cmd_args_data* cmd) {
    if (!m_lines || !cmd) return;

    size_t n_files = get_count_of_files_with_at_least_one_matched_line(m_lines);
    const flags* f = &cmd->flags;
    if (n_files > 0 && f->l) {
        print_file_names_with_at_least_one_matched_line(m_lines);
    } else if (n_files > 0) {
        if (f->c) {
            for (size_t i = 0; cmd->search_files[i] != NULL; ++i) {
                printf("%s:", cmd->search_files[i]);
                size_t n = get_matched_lines_count_on_file(cmd->search_files[i], m_lines);
                if (n != __SIZE_MAX__)
                    printf("%ld\n", n);
                else
                    printf("0\n");
            }
        } else {
            size_t n_lines = get_matched_lines_count(m_lines);
            for (size_t i = 0; i < n_lines; ++i) {
                if (n_files > 1 && !f->h) {
                    printf("%s:", m_lines[i].file_name);
                }
                if (f->n) {
                    printf("%d:", m_lines[i].line_number);
                }
                printf("%s\n", m_lines[i].line);
            }
        }
    }
}

void print_file_names_with_at_least_one_matched_line(const matched_line* m_lines) {
    if (!m_lines) return;

    const char* fn = NULL;
    while ((fn = get_next_file_name(fn, m_lines)) != NULL) printf("%s\n", fn);
}