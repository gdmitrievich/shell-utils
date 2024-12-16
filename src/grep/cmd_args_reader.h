#ifndef CMD_ARGS_READER_H
#define CMD_ARGS_READER_H

#include "../common/common.h"
#include "cmd_args_data.h"

bool set_retrieved_cmd_arg_data(cmd_args_data* cad, int argc, char** argv);

#endif  // CMD_ARGS_READER_H