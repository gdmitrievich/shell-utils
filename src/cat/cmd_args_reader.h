#ifndef CMD_ARGS_READER_H
#define CMD_ARGS_READER_H

#include "flags.h"
#include "../common/common.h"

bool set_cmd_arg_flags(flags* f, int argc, char** argv);
bool set_idx_of_first_filepath(int* idx, int argc, char** argv);

#endif  // CMD_ARGS_READER_H