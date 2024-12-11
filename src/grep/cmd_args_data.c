#include "cmd_args_data.h"

#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void init_cad(cmd_args_data* cad) {
    cad->flags = (flags){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    cad->pattern = cad->pattern_file = NULL;
    cad->argv = NULL;
    cad->n_search_files = 0;
}

void free_cad(cmd_args_data cad) {
    free(cad.pattern);
    free(cad.pattern_file);
}