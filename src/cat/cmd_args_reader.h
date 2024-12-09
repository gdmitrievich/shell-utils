#ifndef CMD_ARGS_READER_H
#define CMD_ARGS_READER_H

#include "flags.h"

flags get_cmd_arg_flags(int argc, char** argv);
int get_idx_of_first_filepath(int argc, char** argv);

#endif // CMD_ARGS_READER_H