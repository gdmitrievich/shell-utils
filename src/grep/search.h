#ifndef SEARCH_H
#define SEARCH_H

#include "cmd_args_data.h"

void set_regex_matches(cmd_args_data* cad);

void set_retrieved_regexes_from_file(char** pattern_ptr, const char* pattern_file);
void set_reg_exec_results(const cmd_args_data* cmd);

#endif  // SEARCH_H