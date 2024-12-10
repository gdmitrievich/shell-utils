#include "cmd_args_reader.h"
#include "flag_performer.h"

int main(int argc, char** argv) {
    flags f = get_cmd_arg_flags(argc, argv);
    int f_idx = get_idx_of_first_filepath(argc, argv);
    process_flags(f, f_idx, argc, argv);
    return 0;
}
