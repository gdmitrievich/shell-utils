#include "cmd_args_data.h"

#include <string.h>

void init(cmd_args_data* cad) {
    cad->flags = (flags){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    memset(cad->pattern, 0, sizeof(cad->pattern));
    memset(cad->pattern_file, 0, sizeof(cad->pattern_file));
    memset(cad->search_files, 0, sizeof(cad->search_files));
    cad->n_search_files = 0;
}