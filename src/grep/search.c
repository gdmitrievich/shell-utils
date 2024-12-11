#include "search.h"

void set_regex_matches(regex_matches* rm, cmd_args_data* cad) {
    set_retrieved_regexes_from_file(cad->pattern, cad->pattern_file);
    set_reg_exec_results(cad->pattern, cad->flags, rm);
}