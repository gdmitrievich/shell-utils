#include "cmd_args_reader.h"
#include "flag_performer.h"

int main(int argc, char** argv) {
    flags f;
	init_flags(&f);
    bool status = set_cmd_arg_flags(&f, argc, argv);
    int f_idx = 0;
    if (status) status = set_idx_of_first_filepath(&f_idx, argc, argv);
    if (status) process_flags(f, f_idx, argc, argv);
    return 0;
}
