#include <errno.h>
#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "matched_line.h"
#include "output.h"
#include "search.h"

int main(int argc, char** argv) {
    cmd_args_data cad;
    init_cad(&cad);
    bool status = set_retrieved_cmd_arg_data(&cad, argc, argv);
    if (status) {
        printf("flags: ");
        if (cad.flags.e) printf("e");
        if (cad.flags.i) printf("i");
        if (cad.flags.v) printf("v");
        if (cad.flags.c) printf("c");
        if (cad.flags.l) printf("l");
        if (cad.flags.n) printf("n");
        if (cad.flags.h) printf("h");
        if (cad.flags.s) printf("s");
        if (cad.flags.f) printf("f");
        if (cad.flags.o) printf("o");
        printf("\npatterns: %s\n", cad.patterns);
        printf("pattern_files: ");
        for (size_t i = 0; cad.pattern_files && cad.pattern_files[i] != NULL; ++i) {
            printf("%s, ", cad.pattern_files[i]);
        }
        printf("\nsearch_files: ");
        for (size_t i = 0; cad.search_files && cad.search_files[i] != NULL; ++i) {
            printf("%s, ", cad.search_files[i]);
        }
        printf("\nstatus: %d", status);
    }
    return 0;
    // matched_line* m_lines = NULL;
    // if (status) status = set_matched_lines(&m_lines, &cad);
    // if (status) output(m_lines, &cad);

    // free_matched_lines(m_lines);
    // free_cad(cad);

    // if (!status && errno != 0) print_error("grep", NULL);
    return 0;
}
