#include "cmd_args_data.h"

#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void init_cad(cmd_args_data* cad) {
    cad->flags = (flags){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    cad->patterns = NULL;
    cad->search_files = cad->pattern_files = NULL;
}

void free_cad(cmd_args_data cad) {
    free(cad.patterns);
    free_arr_of_strings(cad.pattern_files);
    free_arr_of_strings(cad.search_files);
}

void free_arr_of_strings(char** str_arr) {
    if (!str_arr) return;

    for (int i = 0; str_arr[i] != NULL; ++i) free(str_arr[i]);
    free(str_arr);
}