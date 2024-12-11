#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "regex_matches.h"
#include "search.h"

int main(int argc, char** argv) {
    cmd_args_data cad = retrieve_cmd_arg_data(argc, argv);
    regex_matches rm;
    init_rm(&rm);
    set_regex_matches(&rm, &cad);
    free_rm(rm);
    free_cad(cad);
    return 0;
}
