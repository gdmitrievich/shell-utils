#include <errno.h>
#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "matched_line.h"
#include "output.h"
#include "search.h"
#include "s21_grep.h"

int main(int argc, char** argv) {
	s21_grep(argc, argv);
	return 0;
}

void s21_grep(int argc, char** argv) {
    cmd_args_data cad;
    init_cad(&cad);
    bool status = set_retrieved_cmd_arg_data(&cad, argc, argv);
    matched_line* m_lines = NULL;
    if (status) status = set_matched_lines(&m_lines, &cad);
    if (status) output(m_lines, &cad);

    free_matched_lines(m_lines);
    free_cad(cad);

    if (!status && errno != 0 && !(cad.flags.s && errno == ENOENT)) print_error("grep", NULL);
}