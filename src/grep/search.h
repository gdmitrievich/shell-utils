#ifndef SEARCH_H
#define SEARCH_H

#include "cmd_args_data.h"
#include "regex_matches.h"

void set_regex_matches(regex_matches* rm, cmd_args_data* cad);

void set_retrieved_regexes_from_file(char** pattern_ptr, const char* pattern_file);
void set_reg_exec_results(const cmd_args_data* cmd, regex_matches* regexms);

#endif  // SEARCH_H