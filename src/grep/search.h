#ifndef SEARCH_H
#define SEARCH_H

#include "cmd_args_data.h"
#include "matched_line.h"

matched_line* get_matched_lines(cmd_args_data* cad);

void set_retrieved_regexes_from_file(char** pattern_ptr, const char* pattern_file);
matched_line* get_reg_exec_results_as_matched_lines(const cmd_args_data* cmd);

#endif  // SEARCH_H