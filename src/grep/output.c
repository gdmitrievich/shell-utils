#include "output.h"

#include <stdio.h>

void output(const matched_line* m_lines, const cmd_args_data* cad, const char* search_file) {
    if (!cad) return;

    if (cad->flags.l) {
        size_t n = get_matched_lines_count_on_file(search_file, m_lines);
        if (n > 0 && n != __SIZE_MAX__) printf("%s\n", search_file);
    } else {
        if (cad->flags.c)
            print_count_of_matched_lines_on_search_file(m_lines, cad, search_file);
        else
            print_matched_lines_with_additional_info_if_needed(m_lines, cad, search_file);
    }
}

size_t get_count_of_files_from_cmd_args(const cmd_args_data* cad) {
    size_t n = 0;
    while (cad->search_files[n] != NULL) ++n;
    return n;
}

void print_count_of_matched_lines_on_search_file(const matched_line* m_lines, const cmd_args_data* cad,
                                                 const char* search_file) {
    size_t n_files = get_count_of_files_from_cmd_args(cad);
    if (n_files > 1 && !cad->flags.h) printf("%s:", search_file);
    size_t n = get_matched_lines_count_on_file(search_file, m_lines);
    if (n != __SIZE_MAX__)
        printf("%ld\n", n);
    else
        printf("0\n");
}

void print_matched_lines_with_additional_info_if_needed(const matched_line* m_lines, const cmd_args_data* cad,
                                                        const char* search_file) {
    if (!m_lines) return;

    size_t n_files = get_count_of_files_from_cmd_args(cad);
    size_t l_idx = find_idx_of_last_matched_line_with_file_name(search_file, m_lines);
    if (l_idx != __SIZE_MAX__) {
        for (size_t i = find_idx_of_first_matched_line_with_file_name(search_file, m_lines); i <= l_idx;
             ++i) {
            if (n_files > 1 && !cad->flags.h) {
                printf("%s:", m_lines[i].file_name);
            }
            if (cad->flags.n) {
                printf("%d:", m_lines[i].line_number);
            }
            printf("%s\n", m_lines[i].line);
        }
    }
}