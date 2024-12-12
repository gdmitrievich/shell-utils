#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "matched_line.h"
#include "search.h"

int main(int argc, char** argv) {
    cmd_args_data cad = retrieve_cmd_arg_data(argc, argv);
    matched_line* m_lines = get_matched_lines(&cad);

    // display_data(m_lines, cad);

    free_matched_lines(m_lines);
    free_cad(cad);
    return 0;
}
