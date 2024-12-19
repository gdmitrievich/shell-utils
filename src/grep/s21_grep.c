#include "s21_grep.h"

#include <errno.h>
#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "matched_line.h"
#include "output.h"
#include "pattern_files_reader.h"
#include "search.h"

int main(int argc, char** argv) {
    s21_grep(argc, argv);
    return 0;
}

void s21_grep(int argc, char** argv) {
    cmd_args_data cad;
    init_cad(&cad);
    bool status = set_retrieved_cmd_arg_data(&cad, argc, argv);
    if (cad.flags.f) status = set_regexes_retrieved_from_files(&cad.patterns, cad.pattern_files);
    if (status) status = process_files_one_by_one_as_grep_utility(&cad);
    free_cad(cad);

    if (!status && errno != 0 && !(cad.flags.s && errno == ENOENT)) print_error("grep", NULL);
}

bool process_files_one_by_one_as_grep_utility(const cmd_args_data* cad) {
    bool status = true;
    for (size_t i = 0; status && cad->search_files[i] != NULL; ++i) {
        matched_line* m_lines = NULL;
		bool file_found = true;
        status = set_matched_lines_with_patterns_from_file(&m_lines, cad, cad->search_files[i], &file_found);
        if (status && file_found) output(m_lines, cad, cad->search_files[i]);
        free_matched_lines(m_lines);
    }
    return status;
}