#ifndef OUTPUT_H
#define OUTPUT_H

#include "cmd_args_data.h"
#include "matched_line.h"

void output(const matched_line* m_lines, const cmd_args_data* cmd);

size_t get_count_of_files_from_cmd_args(const cmd_args_data* cad);

void print_file_names_with_at_least_one_matched_line(const matched_line* m_lines);

#endif  // OUTPUT_H