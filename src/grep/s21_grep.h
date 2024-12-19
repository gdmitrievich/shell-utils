#ifndef S21_GREP_H
#define S21_GREP_H

#include "../common/common.h"
#include "cmd_args_data.h"

void s21_grep(int argc, char** argv);
bool process_files_one_by_one_as_grep_utility(const cmd_args_data* cad);

#endif  // S21_GREP_H