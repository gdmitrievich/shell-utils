#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "search.h"

int main(int argc, char** argv) {
    cmd_args_data cad = retrieve_cmd_arg_data(argc, argv);
    set_regex_matches(&cad);
    free_cad(cad);
    return 0;
}
