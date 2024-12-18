#ifndef SEARCH_H
#define SEARCH_H

#include <regex.h>
#include <stdio.h>

#include "cmd_args_data.h"
#include "matched_line.h"

bool set_matched_lines_with_patterns_from_file(matched_line** m_lines_ptr, const cmd_args_data* cad,
                                               const char* search_file);

bool set_reg_exec_results_as_matched_lines(matched_line** m_lines_ptr, const cmd_args_data* cmd);
bool set_all_matches_from_line(matched_line** m_lines, regex_t* regex, char* str, regmatch_t* rm,
                               const char* file_name, size_t line_number);

#endif  // SEARCH_H