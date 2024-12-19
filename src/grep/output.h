#ifndef OUTPUT_H
#define OUTPUT_H

#include "cmd_args_data.h"
#include "matched_line.h"

void output(const matched_line* m_lines, const cmd_args_data* cad,
            const char* search_file);

size_t get_count_of_files_from_cmd_args(const cmd_args_data* cad);

void print_count_of_matched_lines_on_search_file(const matched_line* m_lines,
                                                 const cmd_args_data* cad,
                                                 const char* search_file);

void print_matched_lines_with_additional_info_if_needed(
    const matched_line* m_lines, const cmd_args_data* cad,
    const char* search_file);

#endif  // OUTPUT_H